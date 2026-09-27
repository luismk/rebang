#!/usr/bin/env python3
"""
Utility for tracing the VC7.1 backend to help with producing matching decomps.

This mainly exists because debugging under Wine can be a bit of a pain.

If you actually have a native Windows box, it is possibly easier to just trace in IDA or x64dbg.

This does have the benefit of making it easier to get the right invocation, though.

Examples:

    # Initialize, including downloading c2.pdb and etc.
    python tools/c2trace.py setup

    # Search for symbols in c2.pdb
    python tools/c2trace.py symbols InlCallGraphDecision OptCmpHashVal

    # Instrument backend while compiling a specific function
    python tools/c2trace.py run frelement LoadXml InlCallGraphDecision 0x10760c81:8

    # Instrument backend while compiling a speciifc function while overriding the source
    python tools/c2trace.py run frelement LoadXml ShouldInlineInlinee --override-source draft.cpp
"""

from __future__ import annotations

import argparse
import bisect
import re
import struct
import subprocess
import sys
import urllib.request
from pathlib import Path

import common
import pdbinfo
import probe

WORK = common.ROOT / ".cache/c2trace"
BACKEND = common.ROOT / "tools/VC7.1/bin/c2.dll"
SYMBOLS = WORK / "c2.symbols"
TRACER = WORK / "c2trace.exe"
PREFERRED_BASE = 0x10700000
SYMBOL_SERVER = "https://msdl.microsoft.com/download/symbols"


def pdb_identity(dll: Path) -> tuple[str, str]:
    data = dll.read_bytes()
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    count, optional = (
        struct.unpack_from("<H", data, pe + 6)[0],
        struct.unpack_from("<H", data, pe + 20)[0],
    )
    rva, size = struct.unpack_from("<II", data, pe + 24 + 96 + 6 * 8)
    sections = [
        struct.unpack_from("<IIII", data, pe + 24 + optional + i * 40 + 8)
        for i in range(count)
    ]
    offset = next(
        raw + rva - va for vsize, va, _, raw in sections if va <= rva < va + vsize
    )
    for i in range(size // 28):
        kind, length, _, pointer = struct.unpack_from(
            "<IIII", data, offset + i * 28 + 12
        )
        record = data[pointer : pointer + length]
        if kind == 2 and record[:4] == b"RSDS":
            a, b, c = struct.unpack_from("<IHH", record, 4)
            age = struct.unpack_from("<I", record, 20)[0]
            name = record[24:].split(b"\0")[0].decode().rsplit("\\", 1)[-1]
            return name, f"{a:08X}{b:04X}{c:04X}{record[12:20].hex().upper()}{age:X}"
    raise SystemExit(f"{dll}: no RSDS debug record")


def fetch_pdb(dll: Path) -> Path:
    name, key = pdb_identity(dll)
    path = WORK / key / name
    if path.exists():
        return path
    url = f"{SYMBOL_SERVER}/{name}/{key}/{name}"
    print(f"downloading {url}", file=sys.stderr)
    path.parent.mkdir(parents=True, exist_ok=True)
    with urllib.request.urlopen(url) as response:
        path.write_bytes(response.read())
    return path


def write_symbols() -> None:
    pdb = common.windows(fetch_pdb(BACKEND))
    omap = sorted(
        (int(a, 16), int(b, 16))
        for a, b in re.findall(
            r"^\s+([0-9A-F]{8})\s+([0-9A-F]{8})\s*$",
            pdbinfo.cvdump("-omapf", pdb),
            re.MULTILINE,
        )
    )
    starts = [a for a, _ in omap]
    headers = pdbinfo.cvdump("-headers", pdb).split("ORIGINAL SECTION HEADERS", 1)[-1]
    section_rvas = [
        int(v, 16)
        for v in re.findall(r"^\s*([0-9A-F]+) virtual address", headers, re.MULTILINE)
    ]
    rows = []
    for m in pdbinfo.PUBLIC.finditer(pdbinfo.cvdump("-p", pdb)):
        section, offset, name = int(m[1], 16), int(m[2], 16), m[3]
        if not 0 < section <= len(section_rvas):
            continue
        rva = section_rvas[section - 1] + offset
        i = bisect.bisect_right(starts, rva) - 1
        if i >= 0 and omap[i][1]:
            rows.append((PREFERRED_BASE + omap[i][1] + rva - omap[i][0], name))
    WORK.mkdir(parents=True, exist_ok=True)
    SYMBOLS.write_text("".join(f"{a:08x} {n}\n" for a, n in sorted(rows)))
    print(f"{len(rows)} symbols -> {SYMBOLS.relative_to(common.ROOT)}")


def build_tracer() -> None:
    source = common.ROOT / "tools/c2trace/main.c"
    obj = WORK / "main.obj"
    sdk = ["tools/VC7.1/include", "tools/PlatformSDK-Aug2002/include"]
    common.invoke(
        "cl.exe",
        obj,
        [
            "/nologo",
            "/c",
            "/MD",
            "/Od",
            *["/I" + common.windows(i) for i in sdk],
            "/Fo" + common.windows(obj),
            common.windows(source),
        ],
    )
    libraries = ["tools/VC7.1/lib", "tools/PlatformSDK-Aug2002/lib"]
    common.invoke(
        "link.exe",
        TRACER,
        [
            "/nologo",
            "/OUT:" + common.windows(TRACER),
            *["/LIBPATH:" + common.windows(i) for i in libraries],
            common.windows(obj),
            "kernel32.lib",
        ],
    )
    print(f"built {TRACER.relative_to(common.ROOT)}")


def symbols() -> list[tuple[int, str]]:
    if not SYMBOLS.exists():
        write_symbols()
    return [
        (int(a, 16), n)
        for a, n in (line.split(" ", 1) for line in SYMBOLS.read_text().splitlines())
    ]


def plain(name: str) -> str:
    return re.sub(r"@\d+$", "", name).lstrip("@_")


def resolve(spec: str, table: list[tuple[int, str]]) -> str:
    spec, _, dump = spec.partition(":")
    if re.fullmatch(r"(0x)?[0-9a-fA-F]{8}", spec):
        address = int(spec, 16)
    else:
        name, _, offset = spec.partition("+")
        found = [a for a, n in table if n == name] or [
            a for a, n in table if plain(n) == plain(name)
        ]
        if len(found) != 1:
            close = [n for _, n in table if plain(name).lower() in n.lower()][:10]
            raise SystemExit(
                f"{name}: {len(found)} matches"
                + (f"; similar: {', '.join(close)}" if close else "")
            )
        address = found[0] + (int(offset, 0) if offset else 0)
    return f"{address:08x}" + (f":{dump}" if dump else "")


def cmd_setup(args: argparse.Namespace) -> int:
    build_tracer()
    write_symbols()
    return 0


def cmd_symbols(args: argparse.Namespace) -> int:
    for address, name in symbols():
        if not args.names or any(k.lower() in name.lower() for k in args.names):
            print(f"{address:08x} {name}")
    return 0


def cmd_run(args: argparse.Namespace) -> int:
    if not TRACER.exists():
        build_tracer()
    print("Please wait. Building with tracing can be slow.")
    table = symbols()
    breakpoints = [resolve(b, table) for b in args.breakpoints]
    setup = probe.make_setup(args)
    obj, log = probe.compile_variant(setup, "trace", [])
    if obj is None:
        print(log)
        return 1
    rsp = obj.with_suffix(".obj.rsp")
    command = WORK / "trace.cmd"
    command.write_text(
        f'"{common.windows(common.BIN / "cl.exe")}" @"{probe.windows(rsp)}"'
    )
    function = args.function
    if not function.startswith(("?", "*", "_", "@")):
        function = f"?{function}@"
    result = subprocess.run(
        ["wine", str(TRACER), common.windows(command), function, *breakpoints],
        cwd=common.ROOT,
        env=common.wine_environment(),
        stdout=sys.stdout,
        text=True,
        errors="replace",
        check=False,
    )
    return result.returncode


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Debug tool for COFF symbols and COMDAT flags"
    )
    sub = parser.add_subparsers(dest="cmd", required=True)

    p = sub.add_parser("setup")
    p.set_defaults(func=cmd_setup)

    p = sub.add_parser("symbols", help="search compiler symbols")
    p.add_argument("names", nargs="*")
    p.set_defaults(func=cmd_symbols)

    p = sub.add_parser("run", help="trace compilation")
    p.add_argument("unit")
    p.add_argument("function")
    p.add_argument("breakpoints", nargs="+")
    p.add_argument("--source")
    p.add_argument("--flags")
    p.add_argument("--add-flags")
    p.add_argument("--remove-flags")
    p.add_argument("--pch")
    p.set_defaults(func=cmd_run)

    args = parser.parse_args(argv)
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
