#!/usr/bin/env python3
"""Build tool for Rebang

On Linux the original MSVC 7.1 toolchain runs under Wine and the build is driven
by GNU Make (``make``). On Windows the same toolchain runs natively (no Wine)
and the build can be driven without Make:

    python tools\\build.py build            # build every target
    python tools\\build.py rebuild          # clean, then build
    python tools\\build.py -t projectg build
    python tools\\build.py verify
"""

from __future__ import annotations

import argparse
import functools
import hashlib
import json
import os
import re
import shutil
import stat
import subprocess
import sys
import tarfile
import tempfile
import threading
import zipfile
from collections.abc import Callable, Iterable, Iterator, Mapping
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path, PurePosixPath
from typing import BinaryIO, cast

WINDOWS = sys.platform == "win32"

if sys.platform == "win32":
    # common.py imports fcntl, which does not exist on Windows. Provide the small
    # part of it that the build uses (an exclusive lock on a file handle).
    import msvcrt
    import types

    def _flock(handle: object, operation: int) -> None:
        fd = handle.fileno()  # type: ignore[attr-defined]
        if operation & 8:  # LOCK_UN
            msvcrt.locking(fd, msvcrt.LK_UNLCK, 1)
            return
        while True:  # LK_LOCK gives up after ~10 seconds; keep waiting
            try:
                msvcrt.locking(fd, msvcrt.LK_LOCK, 1)
                return
            except OSError:
                continue

    _fcntl = types.ModuleType("fcntl")
    _fcntl.LOCK_EX = 2  # type: ignore[attr-defined]
    _fcntl.LOCK_UN = 8  # type: ignore[attr-defined]
    _fcntl.flock = _flock  # type: ignore[attr-defined]
    sys.modules["fcntl"] = _fcntl

import common  # noqa: E402
import imgcmp  # noqa: E402

# --- Native Windows toolchain support ---------------------------------------
# On Windows, replace the Wine based helpers in common.py with native ones.

_prepare_lock = threading.Lock()
_pdb_drive_ready: bool | None = None


def pdb_drive_ready() -> bool:
    """Whether the PDB path baked into the original image (D:\\...) is usable."""
    return True if not WINDOWS else bool(_pdb_drive_ready)


def _map_pdb_drive() -> bool:
    drive = (common.ROOT / common.PDB_DRIVE).resolve()
    for t in common.targets().values():
        (common.ROOT / t.pdb).parent.mkdir(parents=True, exist_ok=True)

    def mapped() -> bool:
        try:
            return os.path.exists("D:\\") and os.path.samefile("D:\\", drive)
        except OSError:
            return False

    if mapped():
        return True
    if os.path.exists("D:\\"):
        return False  # D: is a real drive or points somewhere else
    # A stale SUBST left over from a deleted build directory would make the
    # mapping below fail ("Drive already SUBSTed"), so drop it first.
    subprocess.run(["subst", "D:", "/D"], capture_output=True, check=False)
    subprocess.run(["subst", "D:", str(drive)], capture_output=True, check=False)
    return mapped()


def _require_runtime() -> None:
    """The MSVC 7.1 compiler needs MSVCP71.dll, which Wine bundles but Windows
    does not. D2027 ("cannot execute c1xx.dll") is the unhelpful symptom."""
    windir = Path(os.environ.get("SystemRoot", "C:\\Windows"))
    folders = [common.BIN, windir / "SysWOW64", windir / "System32"]
    present = {
        entry.name.lower()
        for folder in folders
        if folder.is_dir()
        for entry in folder.iterdir()
    }
    missing = [n for n in ("msvcp71.dll", "msvcr71.dll") if n not in present]
    if missing:
        raise RuntimeError(
            f"missing {', '.join(missing)}: required by tools/VC7.1/bin/c1.dll and "
            "c1xx.dll (Wine provides it, Windows does not).\n"
            "Copy the 32-bit msvcp71.dll (from Visual Studio .NET 2003 or its "
            f"redistributable) into {common.BIN}"
        )


def _native_prepare() -> None:
    global _pdb_drive_ready
    with _prepare_lock:
        if _pdb_drive_ready is None:
            _require_runtime()
            (common.ROOT / "build").mkdir(exist_ok=True)
            _pdb_drive_ready = _map_pdb_drive()
            if not _pdb_drive_ready:
                print(
                    "WARNING: could not map D: to build/pdb-drive (is D: already in "
                    "use?). The build will work, but the output image will not "
                    "match the original byte for byte.",
                    flush=True,
                )


def _native_windows(p: common.PathArgument) -> str:
    return str((common.ROOT / p).resolve())


def _native_environment() -> dict[str, str]:
    env = dict(os.environ)
    # Keep a Visual Studio developer prompt from leaking modern headers/libs.
    for name in ("INCLUDE", "LIB", "LIBPATH", "CL", "_CL_", "LINK", "_LINK_"):
        env.pop(name, None)
    env["PATH"] = str(common.BIN) + os.pathsep + env.get("PATH", "")
    return env


def _native_invoke(
    tool: str,
    output: common.PathArgument,
    args: list[str] | tuple[str, ...],
    cwd: common.PathArgument = common.ROOT,
) -> str:
    _native_prepare()
    output = Path(output)
    output.parent.mkdir(parents=True, exist_ok=True)
    command = (
        list(args)
        if tool == "rc.exe"
        else ["@" + _native_windows(common.response(output, args))]
    )
    log = output.with_suffix(output.suffix + ".log")
    with log.open("w") as stream:
        result = subprocess.run(
            [str(common.BIN / tool), *command],
            cwd=cwd,
            env=_native_environment(),
            stdout=stream,
            stderr=subprocess.STDOUT,
            check=False,
        )
    log_text = log.read_text(errors="replace")
    if result.returncode:
        raise RuntimeError(f"{tool} failed; see {log}\n{log_text}")
    return log_text


if WINDOWS:
    common.windows = _native_windows
    common.prepare_wine = _native_prepare
    common.wine_environment = _native_environment
    common.invoke = _native_invoke


def remove_tree(path: common.PathArgument, ignore_errors: bool = False) -> None:
    """shutil.rmtree that also removes read-only files (needed on Windows)."""

    def handler(function: Callable[[str], object], name: str, *_: object) -> None:
        try:
            os.chmod(name, stat.S_IWRITE)
            function(name)
        except OSError:
            if not ignore_errors:
                raise

    if not Path(path).exists():
        return
    if sys.version_info >= (3, 12):
        shutil.rmtree(path, onexc=handler)
    else:
        shutil.rmtree(path, onerror=handler)  # type: ignore[arg-type]


def python_command() -> str:
    if WINDOWS:
        return '"' + sys.executable.replace("\\", "/") + '"'
    return "python3"


def source_archives() -> dict[str, tuple[common.Target, common.Entry]]:
    archives: dict[str, tuple[common.Target, common.Entry]] = {}
    for target in common.targets().values():
        config, _ = target.settings()
        for directory, entry in config.get("source_archives", {}).items():
            owner, previous = archives.setdefault(directory, (target, entry))
            if previous != entry:
                raise ValueError(
                    f"{directory}: source archive differs between {owner.name} and {target.name}"
                )
    return archives


def unpack_stamp(directory: str) -> str:
    return str(Path("build", directory) / ".unpacked")


def source_files(archive: common.PathArgument) -> Iterator[tuple[Path, bytes]]:
    def relative(name: str) -> Path:
        p = PurePosixPath(name)
        if p.is_absolute() or ".." in p.parts or len(p.parts) < 2:
            raise ValueError(f"invalid source archive path: {name}")
        return Path(*p.parts[1:])

    if zipfile.is_zipfile(archive):
        with zipfile.ZipFile(archive) as z:
            for member in z.infolist():
                if not member.is_dir():
                    yield relative(member.filename), z.read(member)
    else:
        with tarfile.open(archive) as t:
            for tar_member in t:
                if tar_member.isfile():
                    stream = cast(BinaryIO, t.extractfile(tar_member))
                    yield relative(tar_member.name), stream.read()


def split_lines(data: bytes) -> list[bytes]:
    """Split on LF only, keeping line endings (so CRLF files stay intact)."""
    return re.findall(rb"[^\n]*\n|[^\n]+", data)


def apply_patch(root: Path, patch_file: Path) -> None:
    """Minimal `patch --binary --fuzz=0 -p1` for unified diffs.

    Used when GNU patch is unavailable (e.g. on Windows).
    """
    lines = patch_file.read_bytes().split(b"\n")
    header = re.compile(rb"@@ -(\d+)(?:,(\d+))? \+(\d+)(?:,(\d+))? @@")

    def target_path(raw: bytes) -> str | None:
        name = raw.split(b"\t", 1)[0].strip().decode("utf-8")
        if name == "/dev/null":
            return None
        parts = PurePosixPath(name).parts[1:]  # -p1
        if not parts or ".." in parts:
            raise ValueError(f"invalid path in patch: {name}")
        return str(Path(*parts))

    i = 0
    while i < len(lines):
        if not (
            lines[i].startswith(b"--- ")
            and i + 1 < len(lines)
            and lines[i + 1].startswith(b"+++ ")
        ):
            i += 1
            continue
        old_name, new_name = target_path(lines[i][4:]), target_path(lines[i + 1][4:])
        i += 2
        hunks: list[tuple[int, list[bytes], list[bytes]]] = []
        while i < len(lines) and lines[i].startswith(b"@@"):
            m = header.match(lines[i])
            if m is None:
                raise ValueError(f"bad hunk header: {lines[i]!r}")
            start = int(m[1])
            old_count = 1 if m[2] is None else int(m[2])
            new_count = 1 if m[4] is None else int(m[4])
            i += 1
            before: list[bytes] = []
            after: list[bytes] = []
            last_tag = b" "
            while i < len(lines) and (
                len(before) < old_count
                or len(after) < new_count
                or lines[i].startswith(b"\\")
            ):
                line = lines[i]
                i += 1
                if line.startswith(b"\\"):  # "\\ No newline at end of file"
                    if last_tag in (b" ", b"-"):
                        before[-1] = before[-1].removesuffix(b"\n")
                    if last_tag in (b" ", b"+"):
                        after[-1] = after[-1].removesuffix(b"\n")
                    continue
                last_tag, text = line[:1] or b" ", line[1:] + b"\n"
                if last_tag in (b" ", b"-"):
                    before.append(text)
                if last_tag in (b" ", b"+"):
                    after.append(text)
            hunks.append((start, before, after))
        if new_name is None:
            if old_name is not None:
                (root / old_name).unlink()
            continue
        path = root / new_name
        origin = root / old_name if old_name is not None else None
        content = (
            split_lines(origin.read_bytes())
            if origin is not None and origin.exists()
            else []
        )
        shift = 0
        for start, before, after in hunks:
            guess = max(start - 1, 0) + shift
            candidates = sorted(
                range(len(content) - len(before) + 1), key=lambda k: abs(k - guess)
            )
            for k in candidates:
                if content[k : k + len(before)] == before:
                    content[k : k + len(before)] = after
                    shift = k + len(after) - (max(start - 1, 0) + len(before))
                    break
            else:
                raise ValueError(f"{patch_file.name}: hunk failed for {new_name}")
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(b"".join(content))
        if origin is not None and origin != path and origin.exists():
            origin.unlink()  # the patch renames the file


def unpack(directory: str) -> None:
    _, entry = source_archives()[directory]
    dest = common.ROOT / directory
    dest.parent.mkdir(parents=True, exist_ok=True)
    print("UNPACK", common.entry_path(entry), flush=True)
    with tempfile.TemporaryDirectory(dir=dest.parent) as temporary:
        tree = Path(temporary) / "source"
        for relative, data in source_files(common.ROOT / common.entry_path(entry)):
            output = tree / relative
            output.parent.mkdir(parents=True, exist_ok=True)
            output.write_bytes(data)
        if isinstance(entry, dict) and "patch" in entry:
            patch_exe = None if WINDOWS else shutil.which("patch")
            if patch_exe:
                subprocess.run(
                    [
                        patch_exe,
                        "--binary",
                        "--batch",
                        "--fuzz=0",
                        "-p1",
                        "-i",
                        str(common.ROOT / entry["patch"]),
                    ],
                    cwd=tree,
                    check=True,
                )
            else:
                apply_patch(tree, common.ROOT / entry["patch"])
        if dest.exists():
            remove_tree(dest)
        tree.rename(dest)
    stamp = Path(unpack_stamp(directory))
    stamp.parent.mkdir(parents=True, exist_ok=True)
    stamp.touch()


def clean() -> None:
    for directory in source_archives():
        dest = common.ROOT / directory
        if dest.exists():
            remove_tree(dest)
    remove_tree(common.ROOT / "build", ignore_errors=True)
    (common.ROOT / "compile_commands.json").unlink(missing_ok=True)


def make_escape(p: common.PathArgument) -> str:
    return (
        str(p)
        .replace("\\", "/")
        .replace("$", "$$")
        .replace("#", "\\#")
        .replace(" ", "\\ ")
    )


@functools.cache
def include_mirror(directory: str) -> str | None:
    """Build lowercase mirror of headers for clangd."""
    source = common.ROOT / directory
    mirror = common.ROOT / "build/include-mirror" / directory
    if not source.is_dir():
        return None
    links = {
        str(p.relative_to(source)).lower(): p
        for p in sorted(source.rglob("*"))
        if p.is_file() and p.suffix.lower() in common.HEADER_SUFFIXES
    }
    links = {n: p for n, p in links.items() if str(p.relative_to(source)) != n}
    remove_tree(mirror, ignore_errors=True)
    if not links:
        return None
    for name, target in links.items():
        link = mirror / name
        link.parent.mkdir(parents=True, exist_ok=True)
        try:
            link.symlink_to(target)
        except (OSError, NotImplementedError):  # no symlink privilege on Windows
            shutil.copyfile(target, link)
    return str(mirror)


def clang_arguments(
    source: str, options: common.EntryOptions, config: common.BuildConfig, output: str
) -> list[str]:
    """Rough translation from MSVC command to clangd driver command."""
    flags = [*options.get("flags", config["cflags"]), *options.get("extra_flags", [])]
    includes = [*config["includes"], *options.get("includes", [])]
    c = Path(source).suffix.lower() == ".c" and "/TP" not in flags
    args = [
        "clang",
        f"--target={common.CLANG_TARGET}",
        f"-fms-compatibility-version={common.MSC_VERSION}",
        "-fms-extensions",
        "-fms-compatibility",
        "-Wno-switch",
        "-x",
        "c" if c else "c++",
        "-std=" + (("c89" if "/Za" in flags else "gnu89") if c else "c++03"),
        "-D_CLANGD=1",
    ]
    for flag in flags:
        if flag[:1] in "-/" and flag[1:2] in ("D", "U"):
            args.append("-" + flag[1:])
        elif flag[:1] in "-/" and flag[1:3] == "FI":
            args += ["-include", flag[3:]]
    args += ["-I" + str(common.ROOT / i) for i in includes]
    args += ["-I" + m for i in includes if (m := include_mirror(i)) is not None]
    args += ["-c", "-o", str(common.ROOT / output), str(common.ROOT / source)]
    return args


def compile_commands() -> None:
    database: list[dict[str, object]] = []
    for target in common.targets().values():
        config, entries = target.settings()
        pch_sources = {
            common.entry_path(e) for e in config.get("precompiled_headers", [])
        }
        for source, entry in entries.items():
            if Path(source).suffix.lower() not in (".c", ".cpp", ".cxx", ".h"):
                continue
            options: common.EntryOptions = entry if isinstance(entry, dict) else {}
            if "member" in options:
                continue
            output = (
                target.pch_product(source)
                if source in pch_sources
                else target.product(entry)
            )
            database.append(
                {
                    "directory": str(common.ROOT),
                    "file": str(common.ROOT / source),
                    "output": str(common.ROOT / output),
                    "arguments": clang_arguments(source, options, config, output),
                }
            )
    (common.ROOT / "compile_commands.json").write_text(
        json.dumps(database, indent=2) + "\n", newline="\n"
    )


def rules() -> None:
    lines = ["# Generated from tools/targets.json.", ""]
    source_targets: list[str] = []
    for directory, (target, entry) in source_archives().items():
        stamp = unpack_stamp(directory)
        files = {
            str(Path(directory) / name)
            for name, _ in source_files(common.ROOT / common.entry_path(entry))
        }
        dependencies = [common.entry_path(entry), str(target.config), "tools/build.py"]
        if isinstance(entry, dict) and "patch" in entry:
            dependencies.append(entry["patch"])
            patch = (common.ROOT / entry["patch"]).read_text()
            for old, new in re.findall(
                r"^--- ([^\t\n]+)(?:\t[^\n]*)?\n\+\+\+ ([^\t\n]+)", patch, re.MULTILINE
            ):
                if old != "/dev/null":
                    files.discard(str(Path(directory, *PurePosixPath(old).parts[1:])))
                if new != "/dev/null":
                    files.add(str(Path(directory, *PurePosixPath(new).parts[1:])))
        targets = [stamp, *sorted(files)]
        source_targets += targets
        lines += [
            "build/rules.mk: " + " ".join(map(make_escape, dependencies)),
            " ".join(map(make_escape, targets))
            + " &: "
            + " ".join(map(make_escape, dependencies)),
            f"\t@{python_command()} tools/build.py unpack {directory}",
            "",
        ]
    lines += [
        "sources: " + " ".join(map(make_escape, source_targets)),
        "all: sources",
        "",
    ]
    tool_deps = " ".join(
        str(p.relative_to(common.ROOT))
        for p in sorted(common.BIN.iterdir())
        if p.is_file()
    )
    outputs: set[str] = set()
    depfiles: list[str] = []
    for target in common.targets().values():
        config, entries = target.settings()
        run = f"\t@{python_command()} tools/build.py -t {target.name}"
        common_deps = f"{target.config} tools/build.py {tool_deps}"
        pch_sources = {
            common.entry_path(e) for e in config.get("precompiled_headers", [])
        }
        for source, entry in entries.items():
            precompile = source in pch_sources
            dest = target.pch_product(source) if precompile else target.product(entry)
            if dest == source:
                continue
            if dest in outputs:
                raise ValueError(f"conflicting sources for {dest}")
            outputs.add(dest)
            options: common.EntryOptions = entry if isinstance(entry, dict) else {}
            dependencies = list(options.get("dependencies", []))
            if "pch" in options:
                dependencies.append(target.pch_product(options["pch"]))
            resource = Path(source).suffix.lower() == ".rc"
            extract = "member" in options
            if resource:
                dependencies += [
                    str(p)
                    for p in Path(source).parent.iterdir()
                    if p.suffix.lower() in (".ico", ".bmp", ".manifest", ".h", ".rc2")
                ]
            if extract:
                action = "extract"
            elif precompile:
                action = "pch"
            elif resource:
                action = "resource"
            else:
                action = "compile"
            lines += [
                f"{dest}: {source} "
                + " ".join(map(make_escape, dependencies))
                + " "
                + common_deps
                + " | sources",
                f"{run} {action} {source}",
                "",
            ]
            if not resource and not extract:
                depfiles.append(dest + ".d")
        for entry in config["inputs"]:
            if not isinstance(entry, dict) or "library" not in entry:
                continue
            dest = target.product(entry)
            members = [target.product(m) for m in entry["members"]]
            lines += [
                f"{dest}: " + " ".join(members) + " " + common_deps,
                f"{run} library {entry['library']}",
                "",
            ]
        lines += [
            f"{target.linked}: "
            + " ".join(target.product(e) for e in config["inputs"])
            + f" {common_deps} {common.TARGETS.relative_to(common.ROOT)}",
            f"{run} link",
            "",
            f"{target.image}: {target.linked} tools/build.py "
            + str(common.TARGETS.relative_to(common.ROOT)),
            f"{run} normalize",
            "",
            f".PHONY: {target.name}",
            f"{target.name}: {target.image}",
            f"all: {target.image}",
            "",
        ]
    if depfiles:
        lines.append("-include " + " ".join(depfiles))
    text = "\n".join(lines) + "\n"
    if WINDOWS:
        # Make wants forward slashes; keep make_escape's "\\#" and "\\ " intact.
        text = re.sub(r"\\(?=[A-Za-z0-9_.\-])", "/", text)
    Path("build/rules.mk").write_text(text, newline="\n")
    compile_commands()


@functools.cache
def tree_files() -> dict[str, Path]:
    """Real-case paths of everything under source/ and tools/, keyed lowercase."""
    return {
        str(p).lower(): p
        for tree in (common.ROOT / "source", common.ROOT / "tools")
        for p in tree.rglob("*")
        if p.is_file()
    }


def compile_source(
    target: common.Target, source: str, precompile: bool = False
) -> None:
    _, entries = target.settings()
    entry = entries[source]
    options: common.EntryOptions = entry if isinstance(entry, dict) else {}
    output = target.pch_product(source) if precompile else target.product(entry)
    flags, includes, _ = target.compile_settings(source)
    pdb = Path(output).with_suffix(".pdb")
    if precompile or "pch" in options:
        pch_source = source if precompile else options["pch"]
        flags += [
            ("/Yc" if precompile else "/Yu") + Path(pch_source).with_suffix(".h").name,
            "/Fp" + common.windows(target.pch_product(pch_source)),
        ]
        pdb = Path(target.pch_product(pch_source)).with_suffix(".pdb")
    obj = output + ".obj" if precompile else output
    args = [
        "/nologo",
        "/c",
        "/showIncludes",
        *flags,
        *["/I" + common.windows(p) for p in includes],
        "/Fo" + common.windows(obj),
        "/Fd" + common.windows(pdb),
        common.windows(source),
    ]
    print("PCH" if precompile else "CL", source, flush=True)
    # Workaround for C1033 errors.
    pdb.parent.mkdir(parents=True, exist_ok=True)
    with pdb.with_suffix(".pdb.lock").open("w") as lock:
        common.fcntl.flock(lock, common.fcntl.LOCK_EX)
        log = common.invoke("cl.exe", output, args)
    headers: list[str] = []
    actual_paths = tree_files()
    for match in re.finditer(r"Note: including file:\s*(.+)", log):
        name = match[1].strip().replace("\\", "/")
        if WINDOWS:
            p = Path(os.path.normpath(match[1].strip()))
            if not p.is_absolute():
                continue
        elif name[:2].lower() == "z:":
            p = Path(name[2:])
        else:
            continue
        p = actual_paths.get(str(p.resolve()).lower(), p)
        if not p.exists():
            raise ValueError(f"cannot resolve included header: {name}")
        try:
            headers.append(str(p.relative_to(common.ROOT)))
        except ValueError:
            headers.append(str(p))
    headers = sorted(set(headers))
    Path(output + ".d").write_text(
        make_escape(output)
        + ": "
        + " ".join(map(make_escape, headers))
        + "\n"
        + "".join(make_escape(h) + ":\n" for h in headers)
    )


def resource(target: common.Target, source: str) -> None:
    _, entries = target.settings()
    output = target.product(entries[source])
    print("RC", source, flush=True)
    common.invoke(
        "rc.exe",
        output,
        [
            "/c",
            "65001",
            "/l",
            "0x412",
            "/fo",
            common.windows(output),
            common.windows(source),
        ],
        cwd=(common.ROOT / source).parent,
    )


def archive_members(
    data: common.ArchiveData,
) -> Iterator[tuple[int, str, common.ArchiveData]]:
    if data[:8] != b"!<arch>\n":
        raise ValueError("not a COFF archive")
    pos, names = 8, b""
    while pos < len(data):
        name = bytes(data[pos : pos + 16]).decode("ascii").rstrip()
        size = int(data[pos + 48 : pos + 58])
        if name == "//":
            names = bytes(data[pos + 60 : pos + 60 + size])
        elif name != "/":
            if name.startswith("/") and name[1:].isdigit():
                name = (
                    names[int(name[1:]) :]
                    .split(b"\0", 1)[0]
                    .split(b"\n", 1)[0]
                    .decode("ascii")
                )
            yield (
                pos,
                name.replace("\\", "/").rstrip("/").rsplit("/", 1)[-1],
                data[pos + 60 : pos + 60 + size],
            )
        pos += 60 + size + (size & 1)


def member_names(data: bytearray, overrides: Mapping[str, str]) -> None:
    found: set[str] = set()
    for pos, name, _ in archive_members(data):
        if name in overrides:
            replacement = (overrides[name] + "/").encode("ascii")
            if len(replacement) > 16:
                raise ValueError(
                    "import member name must fit the short archive name field"
                )
            data[pos : pos + 16] = replacement.ljust(16, b" ")
            found.add(name)
    if found != set(overrides):
        raise ValueError("archive import name did not match an input")


def extract_member(target: common.Target, source: str) -> None:
    _, entries = target.settings()
    entry = cast(common.EntryOptions, entries[source])
    matches = [
        body
        for _, name, body in archive_members(Path(source).read_bytes())
        if name == entry["member"]
    ]
    if len(matches) != 1:
        raise ValueError(f"expected one {entry['member']} in {source}")
    output = Path(target.product(entry))
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(matches[0])
    print("EXTRACT", source, entry["member"], flush=True)


def library(target: common.Target, name: str) -> None:
    config, _ = target.settings()
    entry = next(
        e for e in config["inputs"] if isinstance(e, dict) and e.get("library") == name
    )
    output = target.product(entry)
    print("LIB", name, flush=True)
    members: list[str] = []
    overrides: dict[str, str] = {}
    for member in entry["members"]:
        source = target.product(member)
        if Path(source).suffix.lower() == ".lib":
            directory = Path(output + ".members") / Path(source).stem
            directory.mkdir(parents=True, exist_ok=True)
            for i, (_, original, body) in enumerate(
                archive_members(Path(source).read_bytes())
            ):
                obj = directory / f"{i:03d}_{Path(original).stem}.obj"
                obj.write_bytes(body)
                members.append(str(obj))
                if original.endswith(".dll"):
                    overrides[obj.name] = original
        else:
            members.append(source)
    Path(output).unlink(missing_ok=True)
    common.invoke(
        "lib.exe",
        output,
        [
            "/nologo",
            "/OUT:" + common.windows(output),
            *[common.windows(m) for m in reversed(members)],
        ],
    )
    if overrides:
        data = bytearray(Path(output).read_bytes())
        member_names(data, overrides)
        Path(output).write_bytes(data)


def link(target: common.Target) -> None:
    config, _ = target.settings()
    common.prepare_wine()
    pdb = common.ROOT / target.pdb
    pdb.unlink(missing_ok=True)
    args = [
        *config["link_flags"],
        "/FORCE:MULTIPLE",
        "/OUT:" + common.windows(target.linked),
        "/MAP:" + common.windows(target.image.with_suffix(".map")),
        "/PDB:"
        + (target.pdb_path if pdb_drive_ready() else common.windows(target.pdb)),
        *[common.windows(target.product(e)) for e in config["inputs"]],
    ]
    print("LINK", target.linked, flush=True)
    common.invoke("link.exe", target.linked, args)
    shutil.copyfile(pdb, common.ROOT / target.image.with_suffix(".pdb"))


def show_diff(target: common.Target) -> None:
    if WINDOWS:
        print("(image diff explanation needs Wine and is skipped on Windows)")
        return
    if target.name == common.DEFAULT_TARGET:
        imgcmp.explain(target.linked, limit=20)


def normalize(target: common.Target) -> None:
    data = bytearray(target.linked.read_bytes())
    if not pdb_drive_ready():
        # The PDB path is part of the image, so it cannot match the original.
        temporary = target.image.with_suffix(".tmp")
        temporary.write_bytes(data)
        temporary.replace(target.image)
        print("UNVERIFIED (D: not mapped)", target.image)
        return
    if len(data) != target.size:
        show_diff(target)
        raise ValueError(
            f"unexpected image size: {len(data)}, expected {target.size} (diff: {len(data) - target.size})"
        )
    for offset, value in target.identity:
        replacement = bytes.fromhex(value)
        data[offset : offset + len(replacement)] = replacement
    actual = hashlib.sha256(data).hexdigest()
    if actual != target.sha256:
        show_diff(target)
        raise ValueError(f"image hash mismatch: {actual}")
    temporary = target.image.with_suffix(".tmp")
    temporary.write_bytes(data)
    temporary.replace(target.image)
    print(actual, target.image)


def verify() -> None:
    for target in common.targets().values():
        actual = hashlib.sha256(target.image.read_bytes()).hexdigest()
        if actual != target.sha256:
            raise ValueError(f"{target.image}: image hash mismatch: {actual}")
        print(actual, target.image)


def unescape_make(text: str) -> str:
    return text.replace("\\ ", " ").replace("\\#", "#").replace("$$", "$")


def depfile_headers(output: str) -> list[str]:
    """Headers recorded by compile_source in <output>.d."""
    depfile = common.ROOT / (output + ".d")
    if not depfile.exists():
        return []
    lines = depfile.read_text().splitlines()[1:]
    return [unescape_make(line[:-1]) for line in lines if line.endswith(":")]


def is_stale(output: str, dependencies: Iterable[common.PathArgument]) -> bool:
    destination = common.ROOT / output
    if not destination.exists():
        return True
    stamp = destination.stat().st_mtime_ns
    for dependency in dependencies:
        try:
            if (common.ROOT / dependency).stat().st_mtime_ns > stamp:
                return True
        except FileNotFoundError:
            return True
    return False


Step = tuple[str, list[str], Callable[[], None]]


def run_steps(steps: list[Step], jobs: int) -> None:
    pending = [action for output, deps, action in steps if is_stale(output, deps)]
    if not pending:
        return
    pool = ThreadPoolExecutor(max_workers=max(1, jobs))
    try:
        for future in as_completed([pool.submit(action) for action in pending]):
            future.result()
    except BaseException:
        pool.shutdown(wait=True, cancel_futures=True)
        raise
    pool.shutdown()


def build_target(target: common.Target, jobs: int) -> None:
    """Build one target without Make (same graph that rules() writes for Make)."""
    config, entries = target.settings()
    pch_sources = {common.entry_path(e) for e in config.get("precompiled_headers", [])}
    tools = [str(p.relative_to(common.ROOT)) for p in sorted(common.BIN.iterdir())]
    common_deps = [str(target.config), "tools/build.py", *tools]
    targets_file = str(common.TARGETS.relative_to(common.ROOT))
    phases: dict[str, list[Step]] = {
        name: [] for name in ("extract", "pch", "compile", "resource", "library")
    }
    for source, entry in entries.items():
        precompile = source in pch_sources
        dest = target.pch_product(source) if precompile else target.product(entry)
        if dest == source:
            continue
        options: common.EntryOptions = entry if isinstance(entry, dict) else {}
        deps = [source, *options.get("dependencies", []), *common_deps]
        if "pch" in options:
            deps.append(target.pch_product(options["pch"]))
        if "member" in options:
            phases["extract"].append(
                (dest, deps, lambda s=source: extract_member(target, s))
            )
        elif Path(source).suffix.lower() == ".rc":
            deps += [
                str(p)
                for p in Path(source).parent.iterdir()
                if p.suffix.lower() in (".ico", ".bmp", ".manifest", ".h", ".rc2")
            ]
            phases["resource"].append(
                (dest, deps, lambda s=source: resource(target, s))
            )
        else:
            deps += depfile_headers(dest)
            name = "pch" if precompile else "compile"
            phases[name].append(
                (
                    dest,
                    deps,
                    lambda s=source, p=precompile: compile_source(target, s, p),
                )
            )
    for entry in config["inputs"]:
        if isinstance(entry, dict) and "library" in entry:
            members = [target.product(m) for m in entry["members"]]
            phases["library"].append(
                (
                    target.product(entry),
                    [*members, *common_deps],
                    lambda n=entry["library"]: library(target, n),
                )
            )
    # Same ordering as the Make rules: precompiled headers before their users,
    # libraries after their members, then link and normalize.
    for name in ("extract", "pch", "compile", "resource", "library"):
        run_steps(phases[name], jobs)
    inputs = [target.product(e) for e in config["inputs"]]
    # Separate calls: staleness of the image depends on the freshly linked file.
    run_steps(
        [
            (
                str(target.linked),
                [*inputs, *common_deps, targets_file],
                lambda: link(target),
            )
        ],
        1,
    )
    run_steps(
        [
            (
                str(target.image),
                [str(target.linked), "tools/build.py", targets_file],
                lambda: normalize(target),
            )
        ],
        1,
    )


def build_all(selected: list[common.Target], jobs: int) -> None:
    if WINDOWS:
        common.prepare_wine()  # fail early if the toolchain cannot run
    for directory, (owner, entry) in source_archives().items():
        dependencies = [common.entry_path(entry), str(owner.config), "tools/build.py"]
        if isinstance(entry, dict) and "patch" in entry:
            dependencies.append(entry["patch"])
        if is_stale(unpack_stamp(directory), dependencies):
            unpack(directory)
    for target in selected:
        print("TARGET", target.name, flush=True)
        build_target(target, jobs)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "-t",
        "--target",
        choices=sorted(common.targets()),
        help=f"target to act on (default: $REBANG_TARGET or {common.DEFAULT_TARGET})",
    )
    parser.add_argument(
        "action",
        nargs="?",
        default="build",
        choices=[
            "rules",
            "compile",
            "pch",
            "unpack",
            "extract",
            "resource",
            "library",
            "link",
            "normalize",
            "verify",
            "clean",
            "build",
            "rebuild",
        ],
    )
    parser.add_argument("source", nargs="?")
    parser.add_argument(
        "-j",
        "--jobs",
        type=int,
        default=os.cpu_count() or 1,
        help="parallel jobs for the 'build' action (default: number of CPUs)",
    )
    args = parser.parse_args()
    if (
        args.action in ("compile", "pch", "unpack", "extract", "resource", "library")
        and not args.source
    ):
        parser.error("this action requires a source path")

    os.chdir(common.ROOT)
    if args.target:
        os.environ["REBANG_TARGET"] = args.target
    target = common.target()
    if args.action in ("build", "rebuild"):
        if args.action == "rebuild":
            clean()
        try:
            build_all(
                [target] if args.target else list(common.targets().values()),
                args.jobs,
            )
        except (RuntimeError, ValueError) as error:
            raise SystemExit(f"error: {error}") from None
    elif args.action == "rules":
        rules()
    elif args.action == "compile":
        compile_source(target, args.source)
    elif args.action == "pch":
        compile_source(target, args.source, True)
    elif args.action == "unpack":
        unpack(args.source)
    elif args.action == "extract":
        extract_member(target, args.source)
    elif args.action == "resource":
        resource(target, args.source)
    elif args.action == "library":
        library(target, args.source)
    elif args.action == "link":
        link(target)
    elif args.action == "normalize":
        normalize(target)
    elif args.action == "verify":
        verify()
    elif args.action == "clean":
        clean()
