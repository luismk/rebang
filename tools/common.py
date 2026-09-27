from __future__ import annotations

import fcntl
import json
import os
import subprocess
from collections.abc import Iterable, Sequence
from pathlib import Path
from typing import TypedDict

ROOT = Path(__file__).resolve().parents[1]
PREFIX = Path(os.environ.get("PROJECTG_WINEPREFIX", ROOT / ".wine")).resolve()
CONFIG = ROOT / "build.json"
BIN = ROOT / "tools/VC7.1/bin"
DOCS = ROOT / "docs"
ORIGINAL_DIR = ROOT / "tools" / "original"
ORIGINAL_EXE = ORIGINAL_DIR / "ProjectG_ReleaseQA.exe"
ORIGINAL_PDB = ORIGINAL_DIR / "ProjectG_ReleaseQA.pdb"
IMAGE = Path("build/ProjectG_ReleaseQA.exe")
LINKED = Path("build/ProjectG_ReleaseQA.link.exe")
IMAGE_SIZE = 7618560
EXPECTED = "761252876446178ad5190e78656aa8790b9dbaf2e86a248f31bb4678347e44fc"
HEADER_SUFFIXES = ("", ".h", ".hpp", ".hxx", ".inl", ".inc")
CLANG_TARGET = "i386-pc-windows-msvc"
MSC_VERSION = "13.10"
IDENTITY = (
    (320, "e1e6a04e"),
    (6184036, "e1e6a04e"),
    (6594392, "b0c094b607038c45a37f50dcaaff0630"),
)


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


def build_settings() -> tuple[BuildConfig, dict[str, Entry]]:
    config: BuildConfig = json.loads(CONFIG.read_text())
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


def compile_settings(source: str) -> tuple[list[str], list[str], str | None]:
    config, entries = build_settings()
    entry = entries[source]
    options: EntryOptions = entry if isinstance(entry, dict) else {}
    flags = [*options.get("flags", config["cflags"]), *options.get("extra_flags", [])]
    includes = [*config["includes"], *options.get("includes", [])]
    return flags, includes, options.get("pch")


def pch_product(source: str) -> str:
    return str(Path("build", source).with_suffix(".pch"))


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
        drive = ROOT / "build/pdb-drive"
        (drive / "Build/Custom/temp/bin").mkdir(parents=True, exist_ok=True)
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
