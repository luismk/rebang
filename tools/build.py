#!/usr/bin/env python3
"""Build tool for Rebang"""

from __future__ import annotations

import argparse
import fcntl
import functools
import hashlib
import json
import re
import shutil
import subprocess
import tarfile
import tempfile
import zipfile
from collections.abc import Iterator, Mapping
from pathlib import Path, PurePosixPath
from typing import BinaryIO, cast

import common
import imgcmp


def product(entry: common.Entry) -> str:
    if isinstance(entry, dict) and "library" in entry:
        return "build/" + entry["library"]
    source = common.entry_path(entry)
    if isinstance(entry, dict) and "member" in entry:
        return str(Path("build", source).with_suffix("") / entry["member"])
    suffix = Path(source).suffix.lower()
    if suffix == ".rc":
        return str(Path("build", source).with_suffix(".res"))
    elif suffix in (".c", ".cpp", ".cxx", ".h"):
        return str(Path("build", source).with_suffix(".obj"))
    else:
        return source


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


def unpack(directory: str) -> None:
    entry: common.Entry = json.loads(common.CONFIG.read_text())["source_archives"][
        directory
    ]
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
            subprocess.run(
                [
                    "patch",
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
        if dest.exists():
            shutil.rmtree(dest)
        tree.rename(dest)
    stamp = Path(unpack_stamp(directory))
    stamp.parent.mkdir(parents=True, exist_ok=True)
    stamp.touch()


def clean() -> None:
    for directory in json.loads(common.CONFIG.read_text()).get("source_archives", {}):
        dest = common.ROOT / directory
        if dest.exists():
            shutil.rmtree(dest)
    shutil.rmtree(common.ROOT / "build", ignore_errors=True)
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
    shutil.rmtree(mirror, ignore_errors=True)
    if not links:
        return None
    for name, target in links.items():
        link = mirror / name
        link.parent.mkdir(parents=True, exist_ok=True)
        link.symlink_to(target)
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
    config, entries = common.build_settings()
    pch_sources = {common.entry_path(e) for e in config.get("precompiled_headers", [])}
    database: list[dict[str, object]] = []
    for source, entry in entries.items():
        if Path(source).suffix.lower() not in (".c", ".cpp", ".cxx", ".h"):
            continue
        options: common.EntryOptions = entry if isinstance(entry, dict) else {}
        if "member" in options:
            continue
        output = common.pch_product(source) if source in pch_sources else product(entry)
        database.append(
            {
                "directory": str(common.ROOT),
                "file": str(common.ROOT / source),
                "output": str(common.ROOT / output),
                "arguments": clang_arguments(source, options, config, output),
            }
        )
    (common.ROOT / "compile_commands.json").write_text(
        json.dumps(database, indent=2) + "\n"
    )


def rules() -> None:
    config, entries = common.build_settings()
    lines = ["# Generated from build.json.", ""]
    source_targets: list[str] = []
    for directory, entry in config.get("source_archives", {}).items():
        stamp = unpack_stamp(directory)
        files = {
            str(Path(directory) / name)
            for name, _ in source_files(common.ROOT / common.entry_path(entry))
        }
        dependencies = [common.entry_path(entry), "build.json", "tools/build.py"]
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
            f"\t@python3 tools/build.py unpack {directory}",
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
    common_deps = "build.json tools/build.py " + tool_deps
    outputs: set[str] = set()
    depfiles: list[str] = []
    pch_sources = {common.entry_path(e) for e in config.get("precompiled_headers", [])}
    for source, entry in entries.items():
        precompile = source in pch_sources
        dest = common.pch_product(source) if precompile else product(entry)
        if dest == source:
            continue
        if dest in outputs:
            raise ValueError(f"conflicting sources for {dest}")
        outputs.add(dest)
        options: common.EntryOptions = entry if isinstance(entry, dict) else {}
        dependencies = list(options.get("dependencies", []))
        if "pch" in options:
            dependencies.append(common.pch_product(options["pch"]))
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
            f"\t@python3 tools/build.py {action} {source}",
            "",
        ]
        if not resource and not extract:
            depfiles.append(dest + ".d")
    for entry in config["inputs"]:
        if not isinstance(entry, dict) or "library" not in entry:
            continue
        dest = product(entry)
        members = [product(m) for m in entry["members"]]
        lines += [
            f"{dest}: " + " ".join(members) + " " + common_deps,
            f"\t@python3 tools/build.py library {entry['library']}",
            "",
        ]
    lines += [
        f"{common.LINKED}: "
        + " ".join(product(e) for e in config["inputs"])
        + " "
        + common_deps,
        "\t@python3 tools/build.py link",
        "",
        f"{common.IMAGE}: {common.LINKED} tools/build.py",
        "\t@python3 tools/build.py normalize",
        "",
    ]
    if depfiles:
        lines.append("-include " + " ".join(depfiles))
    Path("build/rules.mk").write_text("\n".join(lines) + "\n")
    compile_commands()


def compile_source(source: str, precompile: bool = False) -> None:
    _, entries = common.build_settings()
    entry = entries[source]
    options: common.EntryOptions = entry if isinstance(entry, dict) else {}
    output = common.pch_product(source) if precompile else product(entry)
    flags, includes, _ = common.compile_settings(source)
    pdb = Path(output).with_suffix(".pdb")
    if precompile or "pch" in options:
        pch_source = source if precompile else options["pch"]
        flags += [
            ("/Yc" if precompile else "/Yu") + Path(pch_source).with_suffix(".h").name,
            "/Fp" + common.windows(common.pch_product(pch_source)),
        ]
        pdb = Path(common.pch_product(pch_source)).with_suffix(".pdb")
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
        fcntl.flock(lock, fcntl.LOCK_EX)
        log = common.invoke("cl.exe", output, args)
    headers: list[str] = []
    actual_paths = {
        str(p).lower(): p
        for tree in (common.ROOT / "source", common.ROOT / "tools")
        for p in tree.rglob("*")
        if p.is_file()
    }
    for match in re.finditer(r"Note: including file:\s*(.+)", log):
        name = match[1].strip().replace("\\", "/")
        if name[:2].lower() == "z:":
            p = Path(name[2:])
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


def resource(source: str) -> None:
    _, entries = common.build_settings()
    output = product(entries[source])
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


def extract_member(source: str) -> None:
    _, entries = common.build_settings()
    entry = cast(common.EntryOptions, entries[source])
    matches = [
        body
        for _, name, body in archive_members(Path(source).read_bytes())
        if name == entry["member"]
    ]
    if len(matches) != 1:
        raise ValueError(f"expected one {entry['member']} in {source}")
    output = Path(product(entry))
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(matches[0])
    print("EXTRACT", source, entry["member"], flush=True)


def library(name: str) -> None:
    config, _ = common.build_settings()
    entry = next(
        e for e in config["inputs"] if isinstance(e, dict) and e.get("library") == name
    )
    output = product(entry)
    print("LIB", name, flush=True)
    members: list[str] = []
    overrides: dict[str, str] = {}
    for member in entry["members"]:
        source = product(member)
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


def link() -> None:
    config, _ = common.build_settings()
    common.prepare_wine()
    pdb = common.ROOT / "build/pdb-drive/Build/Custom/temp/bin/ProjectG_ReleaseQA.pdb"
    pdb.unlink(missing_ok=True)
    args = [
        *config["link_flags"],
        "/OUT:" + common.windows(common.LINKED),
        "/MAP:" + common.windows("build/ProjectG_ReleaseQA.map"),
        "/PDB:d:\\Build\\Custom\\temp\\bin\\ProjectG_ReleaseQA.pdb",
        *[common.windows(product(e)) for e in config["inputs"]],
    ]
    print("LINK", common.LINKED, flush=True)
    common.invoke("link.exe", common.LINKED, args)
    shutil.copyfile(pdb, common.ROOT / "build/ProjectG_ReleaseQA.pdb")


def show_diff() -> None:
    imgcmp.explain(common.LINKED, limit=20)


def normalize() -> None:
    data = bytearray(common.LINKED.read_bytes())
    if len(data) != common.IMAGE_SIZE:
        show_diff()
        raise ValueError(
            f"unexpected image size: {len(data)}, expected {common.IMAGE_SIZE} (diff: {len(data) - common.IMAGE_SIZE})"
        )
    for offset, value in common.IDENTITY:
        replacement = bytes.fromhex(value)
        data[offset : offset + len(replacement)] = replacement
    actual = hashlib.sha256(data).hexdigest()
    if actual != common.EXPECTED:
        show_diff()
        raise ValueError(f"image hash mismatch: {actual}")
    temporary = common.IMAGE.with_suffix(".tmp")
    temporary.write_bytes(data)
    temporary.replace(common.IMAGE)
    print(actual, common.IMAGE)


def verify() -> None:
    actual = hashlib.sha256(common.IMAGE.read_bytes()).hexdigest()
    if actual != common.EXPECTED:
        raise ValueError(f"image hash mismatch: {actual}")
    print(actual, common.IMAGE)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "action",
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
        ],
    )
    parser.add_argument("source", nargs="?")
    args = parser.parse_args()
    if (
        args.action in ("compile", "pch", "unpack", "extract", "resource", "library")
        and not args.source
    ):
        parser.error("this action requires a source path")

    if args.action == "rules":
        rules()
    elif args.action == "compile":
        compile_source(args.source)
    elif args.action == "pch":
        compile_source(args.source, True)
    elif args.action == "unpack":
        unpack(args.source)
    elif args.action == "extract":
        extract_member(args.source)
    elif args.action == "resource":
        resource(args.source)
    elif args.action == "library":
        library(args.source)
    elif args.action == "link":
        link()
    elif args.action == "normalize":
        normalize()
    elif args.action == "verify":
        verify()
    elif args.action == "clean":
        clean()
