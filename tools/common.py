from __future__ import annotations

import fcntl
import functools
import json
import os
import subprocess
from collections.abc import Iterable, Sequence
from pathlib import Path, PureWindowsPath
from typing import TypedDict

ROOT = Path(__file__).resolve().parents[1]
PREFIX = Path(os.environ.get("PROJECTG_WINEPREFIX", ROOT / ".wine")).resolve()
TARGETS = ROOT / "tools/targets.json"
DEFAULT_TARGET = "projectg"
BIN = ROOT / "tools/VC7.1/bin"
DOCS = ROOT / "docs"
ORIGINAL_DIR = ROOT / "tools" / "original"
PDB_DRIVE = Path("build/pdb-drive")
HEADER_SUFFIXES = ("", ".h", ".hpp", ".hxx", ".inl", ".inc")
CLANG_TARGET = "i386-pc-windows-msvc"
MSC_VERSION = "13.10"


class EntryOptions(TypedDict, total=False):
    path: str
    library: str
    members: list[Entry]
    member: str
    patch: str
    flags: list[str]
    includes: list[str]
    extra_flags: list[str]
    dependencies: list[str]
    pch: str


Entry = str | EntryOptions
PathArgument = str | Path
ArchiveData = bytes | bytearray


class OptionalBuildConfig(TypedDict, total=False):
    precompiled_headers: list[Entry]
    source_archives: dict[str, Entry]


class BuildConfig(OptionalBuildConfig):
    inputs: list[Entry]
    cflags: list[str]
    includes: list[str]
    link_flags: list[str]


def entry_path(entry: Entry) -> str:
    return entry if isinstance(entry, str) else entry["path"]


class Target:
    def __init__(self, name: str, spec: dict) -> None:
        self.name = name
        self.config = Path(spec["config"])
        self.build = Path("build", name)
        self.image = self.build / spec["image"]
        self.linked = self.image.with_suffix(".link" + self.image.suffix)
        self.size: int = spec["size"]
        self.sha256: str = spec["sha256"]
        self.identity: list[tuple[int, str]] = [tuple(i) for i in spec["identity"]]
        self.pdb_path: str = spec["pdb_path"]
        self.pdb = PDB_DRIVE / PureWindowsPath(self.pdb_path).relative_to("d:\\")
        self.original = ORIGINAL_DIR / spec["image"]
        self.original_pdb = self.original.with_suffix(".pdb")

    def settings(self) -> tuple[BuildConfig, dict[str, Entry]]:
        config: BuildConfig = json.loads((ROOT / self.config).read_text())
        entries = list(config.get("precompiled_headers", []))
        for entry in config["inputs"]:
            if isinstance(entry, dict) and "library" in entry:
                defaults: EntryOptions = {}
                if "flags" in entry:
                    defaults["flags"] = entry["flags"]
                if "includes" in entry:
                    defaults["includes"] = entry["includes"]
                if "extra_flags" in entry:
                    defaults["extra_flags"] = entry["extra_flags"]
                for member in entry["members"]:
                    member_options: EntryOptions
                    if isinstance(member, str):
                        member_options = {"path": member}
                    else:
                        member_options = member
                    merged_options: EntryOptions = {**defaults, **member_options}
                    entries.append(merged_options)
            else:
                entries.append(entry)
        return config, {entry_path(e): e for e in entries}

    def compile_settings(self, source: str) -> tuple[list[str], list[str], str | None]:
        config, entries = self.settings()
        entry = entries[source]
        options: EntryOptions = entry if isinstance(entry, dict) else {}
        flags = [
            *options.get("flags", config["cflags"]),
            *options.get("extra_flags", []),
        ]
        includes = [*config["includes"], *options.get("includes", [])]
        return flags, includes, options.get("pch")

    def product(self, entry: Entry) -> str:
        if isinstance(entry, dict) and "library" in entry:
            return str(self.build / entry["library"])
        source = entry_path(entry)
        if isinstance(entry, dict) and "member" in entry:
            return str((self.build / source).with_suffix("") / entry["member"])
        suffix = Path(source).suffix.lower()
        if suffix == ".rc":
            return str((self.build / source).with_suffix(".res"))
        elif suffix in (".c", ".cpp", ".cxx", ".h"):
            return str((self.build / source).with_suffix(".obj"))
        else:
            return source

    def pch_product(self, source: str) -> str:
        return str((self.build / source).with_suffix(".pch"))


@functools.cache
def targets() -> dict[str, Target]:
    specs: dict[str, dict] = json.loads(TARGETS.read_text())
    return {name: Target(name, spec) for name, spec in specs.items()}


def target(name: str | None = None) -> Target:
    return targets()[name or os.environ.get("REBANG_TARGET", DEFAULT_TARGET)]


def windows(p: PathArgument) -> str:
    return "Z:" + str((ROOT / p).resolve()).replace("/", "\\")


def wine_environment() -> dict[str, str]:
    return dict(
        os.environ,
        WINEPREFIX=str(PREFIX),
        WINEDEBUG="-all",
        WINEDLLOVERRIDES="mscoree,mshtml=",
        WINEPATH=windows(BIN),
    )


def prepare_wine() -> None:
    (ROOT / "build").mkdir(exist_ok=True)
    with (ROOT / "build/wine.lock").open("w") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        marker = PREFIX / ".projectg-ready"
        if not marker.exists():
            with (ROOT / "build/wineboot.log").open("w") as log:
                subprocess.run(
                    ["wineboot", "-u"],
                    env=wine_environment(),
                    stdout=log,
                    stderr=subprocess.STDOUT,
                    check=True,
                )
            marker.touch()
        drive = ROOT / PDB_DRIVE
        for t in targets().values():
            (ROOT / t.pdb).parent.mkdir(parents=True, exist_ok=True)
        link = PREFIX / "dosdevices/d:"
        if link.is_symlink() or link.exists():
            if link.resolve() != drive:
                raise ValueError(f"Wine D: points elsewhere: {link}")
        else:
            link.symlink_to(drive)


def response(output: PathArgument, args: Iterable[PathArgument]) -> Path:
    output = Path(output)
    output.parent.mkdir(parents=True, exist_ok=True)
    rsp = output.with_suffix(output.suffix + ".rsp")
    rsp.write_bytes(
        ("\r\n".join('"' + str(a) + '"' for a in args) + "\r\n").encode("ascii")
    )
    return rsp


def invoke(
    tool: str, output: PathArgument, args: Sequence[str], cwd: PathArgument = ROOT
) -> str:
    prepare_wine()
    output = Path(output)
    output.parent.mkdir(parents=True, exist_ok=True)
    command = args if tool == "rc.exe" else ["@" + windows(response(output, args))]
    log = output.with_suffix(output.suffix + ".log")
    with log.open("w") as stream:
        result = subprocess.run(
            ["wine", str(BIN / tool), *command],
            cwd=cwd,
            env=wine_environment(),
            stdout=stream,
            stderr=subprocess.STDOUT,
            check=False,
        )
    log_text = log.read_text(errors="replace")
    if result.returncode:
        raise RuntimeError(f"{tool} failed; see {log}\n{log_text}")
    return log_text
