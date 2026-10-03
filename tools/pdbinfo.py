#!/usr/bin/env python3
"""
cvdump wrapper

Usage:

    # Overview of a module
    python tools/pdbinfo.py unit frelement

    # Line table dump for function
    python tools/pdbinfo.py lines frelement LoadXml

    # Dump of locals via stack records
    python tools/pdbinfo.py locals frelement LoadXml

    # Dump of type declarations
    python tools/pdbinfo.py type FrElementDoc FrGuiItem

    # Where a public name or an address lives
    python tools/pdbinfo.py public '?Load@FrElementDoc@@QAE_NPBD@Z' 0x8e8170 0001:004e7170

    # Compare line tables of a built function to an original
    python tools/pdbinfo.py linecmp build/projectg/source/client/Fresh/source/frelement.obj LoadXml

    # Compare locals of a built object to an original
    python tools/pdbinfo.py localcmp build/projectg/source/client/Fresh/source/frelement.obj
"""

from __future__ import annotations

import argparse
import bisect
import functools
import re
import subprocess
import sys
from collections.abc import Iterable, Iterator
from dataclasses import dataclass, field
from pathlib import Path

import common
import original

CVDUMP = common.ROOT / "tools/cvdump/cvdump.exe"
CACHE = common.ROOT / ".cache/pdb"

RECORD = re.compile(r"^\(([0-9A-F]+)\)\s+(S_\w+):?\s*(.*)$")
DATA = re.compile(r"\[([0-9A-F]{4}):([0-9A-F]{8})\], Type:\s+\S+?,\s*(.*)$")
PROC = re.compile(
    r"\[([0-9A-F]{4}):([0-9A-F]{8})\], Cb: ([0-9A-F]{8}), Type:\s+(\S+), (.*)$"
)
LINE_BLOCK = re.compile(
    r"^\s+(.+?) \((?:None|MD5)[^)]*\), ([0-9A-F]{4}):([0-9A-F]{8})-[0-9A-F]{8}, line/addr"
)
PAIR = re.compile(r"(\d+) ([0-9A-F]{8})")
PUBLIC = re.compile(r"S_PUB32: \[([0-9A-F]{4}):([0-9A-F]{8})\], Flags: \w+, (\S+)")


def cvdump(*args: str) -> str:
    common.prepare_wine()
    result = subprocess.run(
        ["wine", str(CVDUMP), *args],
        cwd=common.ROOT,
        env=common.wine_environment(),
        capture_output=True,
        text=True,
        errors="replace",
        check=False,
    )
    if result.returncode:
        raise RuntimeError(f"cvdump {' '.join(args)} failed:\n{result.stderr}")
    return result.stdout


def cached(name: str, *args: str) -> str:
    pdb = common.target().original_pdb
    path = CACHE / common.target().name / f"{name}.txt"
    if not path.exists() or path.stat().st_mtime < pdb.stat().st_mtime:
        text = cvdump(*args, common.windows(pdb))
        path.parent.mkdir(parents=True, exist_ok=True)
        temporary = path.with_suffix(".tmp")
        temporary.write_text(text)
        temporary.replace(path)
    return path.read_text(errors="replace")


@dataclass
class Publics:
    by_name: dict[str, tuple[int, int]]
    by_address: dict[tuple[int, int], list[str]]

    def names_at(self, section: int, offset: int) -> list[str]:
        return self.by_address.get((section, offset), [])


def parse_publics(text: str) -> Publics:
    by_name: dict[str, tuple[int, int]] = {}
    by_address: dict[tuple[int, int], list[str]] = {}
    for m in PUBLIC.finditer(text):
        address = (int(m[1], 16), int(m[2], 16))
        by_name.setdefault(m[3], address)
        by_address.setdefault(address, []).append(m[3])
    return Publics(by_name, by_address)


@functools.cache
def publics() -> Publics:
    return parse_publics(cached("publics", "-p"))


@dataclass
class Local:
    kind: str
    name: str
    type: str
    where: str

    @property
    def offset(self) -> int | None:
        if self.kind != "S_BPREL32":
            return None
        value = int(self.where, 16)
        return value - (1 << 32) if value & 0x80000000 else value

    def __str__(self) -> str:
        if self.offset is not None:
            return f"{self.name}[{self.offset:+#x}]"
        return f"{self.name}({self.where})"


@dataclass
class Proc:
    name: str
    section: int
    offset: int
    size: int
    is_global: bool
    frame_pointer: bool = False
    frame_size: int = 0
    exception_handler: bool = False
    locals: list[Local] = field(default_factory=list)
    statics: list[tuple[str, int, int]] = field(default_factory=list)
    lines: list[tuple[int, int, str]] = field(default_factory=list)

    @property
    def end(self) -> int:
        return self.offset + self.size

    @property
    def file(self) -> str:
        return self.lines[0][2] if self.lines else ""

    def relative_lines(self) -> list[tuple[int, int]]:
        if not self.lines:
            return []
        base = self.lines[0][0]
        return [(line - base, offset) for line, offset, _ in self.lines]


@dataclass
class Symbols:
    compile: dict[str, str] = field(default_factory=dict)
    procs: list[Proc] = field(default_factory=list)
    data: list[tuple[str, int, int, str]] = field(default_factory=list)

    def find(self, key: str | None) -> list[Proc]:
        return [p for p in self.procs if not key or key in p.name]


def parse_symbols(text: str) -> Symbols:
    out = Symbols()
    symbols, _, lines = text.partition("*** LINES")
    scopes: list[Proc | None] = []
    kind = None
    for raw in symbols.splitlines():
        line = raw.strip()
        m = RECORD.match(line)
        proc = next((p for p in reversed(scopes) if p is not None), None)
        if not m:
            if kind == "S_COMPILE2" and ":" in line:
                key, _, value = line.partition(":")
                out.compile.setdefault(key.strip(), value.strip())
            elif kind in ("S_GPROC32", "S_LPROC32") and proc:
                proc.frame_pointer |= line.startswith("Flags:") and "Frame Ptr" in line
            elif kind == "S_FRAMEPROC" and proc:
                if line.startswith("Frame size"):
                    proc.frame_size = int(line.split("=")[1].split()[0], 16)
                elif line.startswith("Address of exception handler"):
                    proc.exception_handler = not line.endswith("0000:00000000")
            continue
        _, kind, rest = m.groups()
        if kind in ("S_GPROC32", "S_LPROC32"):
            pm = PROC.search(rest)
            if pm:
                p = Proc(
                    pm[5].strip(),
                    int(pm[1], 16),
                    int(pm[2], 16),
                    int(pm[3], 16),
                    kind == "S_GPROC32",
                )
                out.procs.append(p)
            scopes.append(p if pm else None)
        elif kind in ("S_BLOCK32", "S_WITH32", "S_THUNK32"):
            scopes.append(None)
        elif kind == "S_END":
            if scopes:
                scopes.pop()
        elif kind in ("S_BPREL32", "S_REGISTER", "S_REGREL32") and proc:
            head, _, name = rest.rpartition(", ")
            where, _, typ = head.partition(", Type:")
            proc.locals.append(
                Local(kind, name.strip(), typ.strip(), where.strip("[] "))
            )
        elif kind in ("S_LDATA32", "S_GDATA32", "S_LTHREAD32", "S_GTHREAD32"):
            dm = DATA.search(rest)
            if not dm or not dm[3]:
                continue
            if proc:
                proc.statics.append((dm[3], int(dm[1], 16), int(dm[2], 16)))
            else:
                out.data.append((dm[3], int(dm[1], 16), int(dm[2], 16), kind[2:]))
    procs = sorted(out.procs, key=lambda p: (p.section, p.offset))
    starts = [(p.section, p.offset) for p in procs]
    file, section = "", 0
    for raw in lines.splitlines():
        bm = LINE_BLOCK.match(raw)
        if bm:
            file, section = bm[1].replace("\\", "/").rsplit("/", 1)[-1], int(bm[2], 16)
            continue
        for number, address in PAIR.findall(raw) if file else ():
            addr = int(address, 16)
            i = bisect.bisect_right(starts, (section, addr)) - 1
            if i >= 0 and procs[i].section == section and addr < procs[i].end:
                procs[i].lines.append((int(number), addr - procs[i].offset, file))
    for p in out.procs:
        p.lines.sort(key=lambda t: t[1])
    return out


@functools.cache
def module_symbols(number: int) -> Symbols:
    index = original.modules()[number].cvdump
    return parse_symbols(cached(f"M{index}", "-s", "-l", f"-M{index}"))


@dataclass
class Ours:
    symbols: Symbols
    publics: Publics


def ours(obj: Path) -> Ours:
    dll = CACHE / "ours" / obj.stem / f"{obj.stem}.dll"
    pdb = dll.with_suffix(".pdb")
    pdb.unlink(missing_ok=True)
    common.invoke(
        "link.exe",
        dll,
        [
            "/nologo",
            "/DLL",
            "/NOENTRY",
            "/DEBUG",
            "/FORCE:UNRESOLVED",
            "/INCREMENTAL:NO",
            "/NODEFAULTLIB",
            "/OUT:" + common.windows(dll),
            "/PDB:" + common.windows(pdb),
            common.windows(obj.resolve()),
        ],
    )
    path = common.windows(pdb)
    return Ours(
        parse_symbols(cvdump("-s", "-l", path)), parse_publics(cvdump("-p", path))
    )


def proc_keys(procs: Iterable[Proc], table: Publics) -> dict[str, Proc]:
    """Procs keyed by decorated public name, or by plain name for statics."""
    out: dict[str, Proc] = {}
    for p in procs:
        names = table.names_at(p.section, p.offset)
        key = names[0] if names else f"static {p.name}"
        while key in out:
            key += "'"
        out[key] = p
    return out


def pair_procs(
    obj: Path, module: original.Module
) -> Iterator[tuple[Proc | None, Proc | None]]:
    """(original, ours) pairs, in original address order, then ours-only."""
    mine = ours(obj)
    theirs = proc_keys(
        sorted(module_symbols(module.number).procs, key=lambda p: p.offset), publics()
    )
    mine_keys = proc_keys(mine.symbols.procs, mine.publics)
    for key, proc in theirs.items():
        yield proc, mine_keys.pop(key, None)
    for proc in mine_keys.values():
        yield None, proc


PRIMITIVES = {
    "T_VOID": ("void", 0),
    "T_NOTYPE": ("...", 0),
    "T_CHAR": ("signed char", 1),
    "T_RCHAR": ("char", 1),
    "T_UCHAR": ("unsigned char", 1),
    "T_WCHAR": ("wchar_t", 2),
    "T_INT1": ("signed char", 1),
    "T_UINT1": ("unsigned char", 1),
    "T_SHORT": ("short", 2),
    "T_USHORT": ("unsigned short", 2),
    "T_INT2": ("short", 2),
    "T_UINT2": ("unsigned short", 2),
    "T_LONG": ("long", 4),
    "T_ULONG": ("unsigned long", 4),
    "T_INT4": ("int", 4),
    "T_UINT4": ("unsigned int", 4),
    "T_QUAD": ("__int64", 8),
    "T_UQUAD": ("unsigned __int64", 8),
    "T_INT8": ("__int64", 8),
    "T_UINT8": ("unsigned __int64", 8),
    "T_REAL32": ("float", 4),
    "T_REAL64": ("double", 8),
    "T_REAL80": ("long double", 10),
    "T_BOOL08": ("bool", 1),
    "T_HRESULT": ("HRESULT", 4),
}
TYPE_HEADER = re.compile(
    r"^0x([0-9a-f]+) : Length = \d+, Leaf = 0x[0-9a-f]+ (LF_\w+)(.*)$"
)
TYPE_REF = re.compile(r"(T_\w+)\(\w+\)|0x([0-9A-Fa-f]+)")
FIELD = re.compile(r"list\[\d+\] = ")
UDT_KINDS = ("LF_CLASS", "LF_STRUCTURE", "LF_UNION", "LF_ENUM")
Ref = int | str


class Types:
    def __init__(self, text: str):
        bodies: dict[int, list[str]] = {}
        self.kinds: dict[int, str] = {}
        index = None
        for line in text.splitlines():
            m = TYPE_HEADER.match(line)
            if m:
                index = int(m[1], 16)
                self.kinds[index] = m[2]
                bodies[index] = [m[3].strip()] if m[3].strip() else []
            elif index is not None and line.strip():
                bodies[index].append(line.strip())
        self.bodies = {i: "\n".join(b) for i, b in bodies.items()}
        self.by_name: dict[str, list[int]] = {}
        for i, kind in self.kinds.items():
            if kind in UDT_KINDS and "FORWARD REF" not in self.bodies[i]:
                self.by_name.setdefault(self.udt_name(i), []).append(i)

    def udt_name(self, index: int) -> str:
        m = re.search(
            r"(?:class|enum) name = (.+?)(?:, UDT\(|$)",
            self.bodies[index],
            re.MULTILINE,
        )
        return m[1].strip() if m else "?"

    @staticmethod
    def ref(text: str) -> Ref:
        m = TYPE_REF.search(text)
        if not m:
            return text.strip()
        return m[1] if m[1] else int(m[2], 16)

    def after(self, index: int, label: str) -> Ref:
        return self.ref(self.bodies[index].split(label, 1)[1])

    def size(self, t: Ref) -> int:
        if isinstance(t, str):
            return 4 if t.startswith("T_32P") else PRIMITIVES.get(t, ("", 4))[1]
        kind, body = self.kinds.get(t), self.bodies.get(t, "")
        if kind == "LF_POINTER":
            return 4
        if kind == "LF_MODIFIER":
            return self.size(self.after(t, "modifies type"))
        if kind == "LF_ENUM":
            return self.size(self.after(t, "type ="))
        m = re.search(r"(?:Size|length) = (\d+)", body)
        if kind in UDT_KINDS and "FORWARD REF" in body:
            found = self.by_name.get(self.udt_name(t))
            return self.size(found[0]) if found else 0
        return int(m[1]) if m else 0

    def declare(self, t: Ref, name: str = "") -> str:
        """A C declarator of type t for name."""
        if isinstance(t, str):
            if t.startswith("T_32P"):
                return self.declare("T_" + t[5:], "*" + name)
            return f"{PRIMITIVES.get(t, (t, 0))[0]} {name}".rstrip()
        kind, body = self.kinds.get(t, "?"), self.bodies.get(t, "")
        if kind in UDT_KINDS:
            return f"{compact(self.udt_name(t))} {name}".rstrip()
        if kind == "LF_MODIFIER":
            return f"{body.split(',', 1)[0].strip()} {self.declare(self.after(t, 'modifies type'), name)}"
        if kind == "LF_POINTER":
            element = self.after(t, "Element type :")
            head = body.split("\n", 1)[0]
            mark = "&" if "Reference" in head else "*"
            for qualifier in ("const", "volatile"):
                if head.startswith(qualifier):
                    mark += f"{qualifier} "
            inner = mark + name
            if isinstance(element, int) and self.kinds.get(element) in (
                "LF_ARRAY",
                "LF_PROCEDURE",
                "LF_MFUNCTION",
            ):
                inner = f"({inner})"
            return self.declare(element, inner)
        if kind == "LF_ARRAY":
            element = self.after(t, "Element type =")
            count = self.size(t) // (self.size(element) or 1)
            return self.declare(element, f"{name}[{count}]")
        if kind in ("LF_PROCEDURE", "LF_MFUNCTION"):
            return self.declare(
                self.after(t, "Return type ="), f"{name}({self.arguments(t)})"
            )
        return f"/* {kind} {t:#x} */ {name}".rstrip()

    def arguments(self, t: int) -> str:
        m = re.search(r"Arg list type = (0x[0-9A-Fa-f]+)", self.bodies[t])
        if not m:
            return ""
        args = self.bodies.get(int(m[1], 16), "")
        return ", ".join(
            self.declare(self.ref(a)) for a in re.findall(r"list\[\d+\] = (\S+)", args)
        )

    def fields(self, index: int) -> list[str]:
        return [
            f.replace("\n", " ").strip()
            for f in FIELD.split(self.bodies.get(index, ""))
            if f.strip()
        ]

    def method(self, t: int, name: str, attributes: str, owner: str) -> str:
        body = self.bodies[t]
        static = "STATIC" in attributes
        const = False
        this = re.search(r"This type = (\S+?),", body)
        if this and not static:
            pointer = self.ref(this[1])
            if isinstance(pointer, int) and self.kinds.get(pointer) == "LF_POINTER":
                target = self.after(pointer, "Element type :")
                const = isinstance(target, int) and self.bodies.get(
                    target, ""
                ).startswith("const")
        if name == owner or name.startswith("~"):
            text = f"{name}({self.arguments(t)})"
        else:
            text = self.declare(
                self.after(t, "Return type ="), f"{name}({self.arguments(t)})"
            )
        call = re.search(r"Call type = ([^,\n]+)", body)
        convention = call[1].strip() if call else "?"
        note = (
            ""
            if convention == ("Fast Near" if static else "ThisCall")
            else f"  // {convention}"
        )
        prefix = ("static " if static else "") + (
            "virtual " if "VIRTUAL" in attributes or "INTRO" in attributes else ""
        )
        pure = " = 0" if "PURE" in attributes else ""
        return f"{prefix}{text}{' const' if const else ''}{pure};{note}"

    def declaration(self, index: int) -> str:
        kind, body = self.kinds[index], self.bodies[index]
        name = self.udt_name(index)
        found = re.search(r"field list type (0x[0-9a-f]+)", body)
        fieldlist = int(found[1], 16) if found else 0
        if kind == "LF_ENUM":
            values = []
            for f in self.fields(fieldlist):
                m = re.search(r"value = (.+?), name = '([^']*)'", f)
                if m:
                    value = re.sub(r"^\(LF_\w+\) ", "", m[1]).split("(")[0]
                    values.append(f"    {m[2]} = {value},")
            underlying = self.declare(self.after(index, "type ="))
            return (
                f"// {index:#x}: enum ({underlying})\nenum {name}\n{{\n"
                + "\n".join(values)
                + "\n};"
            )
        keyword = {"LF_CLASS": "class", "LF_STRUCTURE": "struct", "LF_UNION": "union"}[
            kind
        ]
        owner = name.rsplit("::", 1)[-1]
        access = "private" if kind == "LF_CLASS" else "public"
        bases: list[str] = []
        lines: list[str] = []

        def emit(member_access: str | None, text: str) -> None:
            nonlocal access
            if member_access and member_access != access:
                access = member_access
                lines.append(f"{access}:")
            lines.append("    " + text)

        for f in self.fields(fieldlist):
            leaf, _, rest = f.partition(", ")
            first = rest.split(", ", 1)[0]
            member_access = (
                first if first in ("public", "protected", "private") else None
            )
            if leaf == "LF_BCLASS":
                bases.append(
                    f"{first} {self.declare(self.ref(rest.split('type =', 1)[1]))}"
                )
            elif leaf == "LF_VBCLASS":
                bases.append(
                    f"virtual {first} {self.declare(self.ref(rest.split('base type =', 1)[1]))}"
                )
            elif leaf == "LF_IVBCLASS":
                continue
            elif leaf == "LF_VFUNCTAB":
                lines.append("    // vfptr")
            elif leaf == "LF_MEMBER":
                m = re.search(
                    r"type = (\S+?), offset = (\d+)\s+member name = '([^']*)'", rest
                )
                if m:
                    emit(
                        member_access,
                        f"{self.declare(self.ref(m[1]), m[3])};  // {int(m[2]):#x}",
                    )
            elif leaf == "LF_STATICMEMBER":
                m = re.search(r"type = (\S+?)\s+member name = '([^']*)'", rest)
                if m:
                    emit(member_access, f"static {self.declare(self.ref(m[1]), m[2])};")
            elif leaf == "LF_ONEMETHOD":
                m = re.search(r"index = (0x[0-9A-Fa-f]+),.*name = '([^']*)'", rest)
                if m and "compgenx" not in rest:
                    emit(member_access, self.method(int(m[1], 16), m[2], rest, owner))
            elif leaf == "LF_METHOD":
                m = re.search(r"list = (0x[0-9A-Fa-f]+), name = '([^']*)'", rest)
                for entry in self.fields(int(m[1], 16)) if m else ():
                    em = re.search(r"^(\w+), (.*?)(0x[0-9A-Fa-f]+)", entry)
                    if m and em and "compgenx" not in entry:
                        emit(em[1], self.method(int(em[3], 16), m[2], em[2], owner))
            elif leaf == "LF_NESTTYPE":
                m = re.search(r"type = (\S+?), (.*)$", rest)
                if m:
                    lines.append(
                        f"    // nested {m[2]}: {self.declare(self.ref(m[1]))}"
                    )
            else:
                lines.append(f"    // {f}")
        head = f"{keyword} {name}" + (" : " + ", ".join(bases) if bases else "")
        size = self.size(index)
        return (
            f"// {index:#x}: {keyword}, size {size:#x}\n{head}\n{{\n"
            + "\n".join(lines)
            + "\n};"
        )


@functools.cache
def types() -> Types:
    return Types(cached("types", "-t"))


def compact(name: str) -> str:
    # Ugly hacks to make STL names vaguely readable (sometimes loses data)
    name = name.replace(
        "std::basic_string<char,std::char_traits<char>,std::allocator<char> >",
        "std::string",
    )
    for argument in (",std::allocator<", ",std::less<", ",std::char_traits<"):
        while (start := name.find(argument)) >= 0:
            depth, i = 0, start + len(argument) - 1
            while i < len(name):
                depth += {"<": 1, ">": -1}.get(name[i], 0)
                if depth == 0:
                    break
                i += 1
            name = name[:start] + name[i + 1 :]
    return name.replace(" >", ">")


def cmd_unit(args: argparse.Namespace) -> None:
    module = original.find_module(args.unit)
    symbols = module_symbols(module.number)
    print(f"{module}, {module.original_module}; cvdump -M{module.cvdump}")
    unit = next((u for u in original.units() if u.module.number == module.number), None)
    if unit:
        options = {k: v for k, v in unit.options.items() if k != "path"}
        state = "converted" if unit.converted else "delinked"
        print(
            f"{common.target().config}: {unit.source} ({state}){' ' + str(options) if options else ''}"
        )
    c = symbols.compile
    target = c.get("Target processor", "?")
    tell = {"Pentium III": "(/G7)", "Pentium Pro/Pentium II": "(no /G7)"}.get(
        target, ""
    )
    plain = [p for p in symbols.procs if not p.exception_handler]
    framed = sum(p.frame_pointer for p in plain)
    omission = "/Oy-" if framed else "frame pointer omission, no /Oy-"
    print(
        f"compiler {c.get('Frontend Version', '?')}: target {target} {tell}, "
        f"/GS {c.get('Compiled with /GS', '?')}, ebp frames in {framed} of {len(plain)} "
        f"non-EH functions ({omission}); /Op is not recorded"
    )
    counts: dict[int, list[int]] = {}
    for contribution in original.module_contributions().get(module.number, []):
        entry = counts.setdefault(contribution.section, [0, 0])
        entry[0] += 1
        entry[1] += contribution.size
    print(
        "contributions: "
        + ", ".join(
            f"{original.SECTION_NAMES.get(s, s)} {n} ({size:#x})"
            for s, (n, size) in sorted(counts.items())
        )
    )
    table = publics()
    print(
        "\nfunctions in address order; flags: L=static, F=no frame pointer, E=EH, P=no public"
    )
    for p in sorted(symbols.procs, key=lambda p: (p.section, p.offset)):
        numbers = [line for line, _, _ in p.lines]
        where = f"{p.file}:{min(numbers)}-{max(numbers)}" if numbers else "-"
        flags = (
            ("" if p.is_global else "L")
            + ("" if p.frame_pointer else "F")
            + ("E" if p.exception_handler else "")
            + ("" if table.names_at(p.section, p.offset) else "P")
        )
        print(f"  {p.offset:08x} {p.size:5x} {flags:4} {where:30} {compact(p.name)}")
    if symbols.data:
        print("\nmodule-level data:")
        for name, section, offset, kind in sorted(
            symbols.data, key=lambda d: (d[1], d[2])
        ):
            print(f"  {section:04x}:{offset:08x} {kind:8} {name}")
    statics = [(p.name, s) for p in symbols.procs for s in p.statics]
    if statics:
        print("\nfunction statics:")
        for owner, (name, section, offset) in statics:
            print(f"  {section:04x}:{offset:08x} {name} in {owner}")
    initializers = sorted(
        (p.name for p in symbols.procs if re.fullmatch(r"\$E\d+", p.name)),
        key=lambda n: int(n[2:]),
    )
    if initializers:
        print(
            "\ndynamic initializers: "
            + " ".join(initializers)
            + " (numbered in definition order, with gaps from statics that got folded)"
        )
    classes: list[str] = []
    for p in symbols.procs:
        owner = p.name.rpartition("::")[0]
        if (
            owner
            and "<" not in owner
            and not owner.startswith("std")
            and owner not in classes
        ):
            classes.append(owner)
    if classes:
        print(
            "\nclasses with methods here (see `pdbinfo.py type`): " + " ".join(classes)
        )


def cmd_lines(args: argparse.Namespace) -> None:
    module = original.find_module(args.unit)
    for p in module_symbols(module.number).find(args.function):
        print(f"== {p.name} {p.size:#x} ({p.file})")
        print("  " + " ".join(f"{line}@{offset:x}" for line, offset, _ in p.lines))


def cmd_locals(args: argparse.Namespace) -> None:
    module = original.find_module(args.unit)
    t = types()
    for p in module_symbols(module.number).find(args.function):
        print(
            f"== {p.name} (frame {p.frame_size:#x}{'' if p.frame_pointer else ', no frame pointer'})"
        )
        for local in p.locals:
            where = f"{local.offset:+#x}" if local.offset is not None else local.where
            print(f"  {where:>7} {t.declare(t.ref(local.type), local.name)}")
        for name, section, offset in p.statics:
            print(f"  {section:04x}:{offset:08x} static {name}")


def cmd_type(args: argparse.Namespace) -> None:
    t = types()
    for name in args.names:
        found = t.by_name.get(name)
        if not found:
            similar = sorted(n for n in t.by_name if name.lower() in n.lower())[:20]
            print(
                f"// {name}: not found"
                + (f"; similar: {', '.join(similar)}" if similar else "")
            )
        for index in found or ():
            print(t.declaration(index))


def cmd_public(args: argparse.Namespace) -> None:
    import coffsym

    table = publics()
    for key in args.names:
        if re.fullmatch(r"[0-9A-Fa-f]{4}:[0-9A-Fa-f]{8}", key):
            section, offset = (int(x, 16) for x in key.split(":"))
        elif re.fullmatch(r"(0x)?[0-9A-Fa-f]+", key):
            located = original.section_offset(int(key, 16))
            if located is None:
                print(f"{key}: not in the image")
                continue
            section, offset = located
        else:
            address = table.by_name.get(key)
            if address is None:
                print(f"{key}: not a public in the original PDB")
                continue
            section, offset = address
        print(f"{key}: {section:04x}:{offset:08x}")
        for name in table.names_at(section, offset):
            if name != key:
                print(f"  also named {name}")
        c = original.contribution_at(section, offset)
        if c is None:
            print("  no contribution found")
            continue
        start = "" if c.offset == offset else f", +{offset - c.offset:#x} into it"
        print(f"  contribution {c.id} ({c.placeholder}) {c}{start}")
        module = original.modules()[c.module]
        print(f"  {module}")
        unit = next((u for u in original.units() if u.module.number == c.module), None)
        if unit and unit.product.suffix == ".obj" and unit.product.exists():
            names = {
                s.name for s in coffsym.load(unit.product)[0].symbols if s.section > 0
            }
            wanted = table.names_at(c.section, c.offset) + [c.placeholder]
            defined = [n for n in wanted if n in names]
            print(
                f"  {unit.product.relative_to(common.ROOT)} defines {', '.join(defined) or 'neither'}"
            )


def cmd_linecmp(args: argparse.Namespace) -> None:
    obj = Path(args.object)
    module = original.find_module(args.unit or obj.stem)
    identical, only_ours = 0, 0
    for theirs, mine in pair_procs(obj, module):
        if theirs is None:
            only_ours += 1
            continue
        name = compact(theirs.name)
        if args.function and args.function not in name:
            continue
        if mine is None:
            print(f"{name:50} missing from ours")
            continue
        a, b = theirs.relative_lines(), mine.relative_lines()
        if a == b:
            identical += 1
            if not args.function:
                continue
        offsets = [o for _, o in a] == [o for _, o in b]
        verdict = (
            "identical"
            if a == b
            else ("same offsets" if offsets else "DIFFERENT offsets")
        )
        print(f"{name:50} {verdict}")
        print("   orig: " + " ".join(f"{line}@{offset:x}" for line, offset in a))
        print("   ours: " + " ".join(f"{line}@{offset:x}" for line, offset in b))
    print(f"{identical} functions with identical relative line tables")
    if only_ours:
        print(f"{only_ours} functions only in ours")


def cmd_localcmp(args: argparse.Namespace) -> None:
    obj = Path(args.object)
    module = original.find_module(args.unit or obj.stem)
    for theirs, mine in pair_procs(obj, module):
        if theirs is None or mine is None:
            continue
        if args.function and args.function not in theirs.name:
            continue
        a = [str(local) for local in theirs.locals]
        b = [str(local) for local in mine.locals]
        if a == b and not args.function:
            continue
        print(f"== {theirs.name}")
        print("   orig: " + " ".join(a))
        print("   ours: " + " ".join(b))
        names_a = [local.name for local in theirs.locals]
        names_b = [local.name for local in mine.locals]
        if extra := [n for n in names_b if n not in names_a]:
            print("   only ours: " + " ".join(extra))
        if missing := [n for n in names_a if n not in names_b]:
            print("   only original: " + " ".join(missing))


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Convenience wrapper for cvdump")
    sub = parser.add_subparsers(dest="cmd", required=True)

    p = sub.add_parser("unit", help="module overview")
    p.add_argument("unit")
    p.set_defaults(func=cmd_unit)
    for name, func in (("lines", cmd_lines), ("locals", cmd_locals)):
        p = sub.add_parser(name, help=f"original {name}")
        p.add_argument("unit")
        p.add_argument("function", nargs="?")
        p.set_defaults(func=func)

    p = sub.add_parser("type", help="type declarations")
    p.add_argument("names", nargs="+")
    p.set_defaults(func=cmd_type)

    p = sub.add_parser("public", help="locate public names/addresses")
    p.add_argument("names", nargs="+")
    p.set_defaults(func=cmd_public)

    p = sub.add_parser(
        "linecmp", help="compare line tables of an object with the original"
    )
    p.add_argument("object")
    p.add_argument("function", nargs="?")
    p.add_argument("--unit")
    p.set_defaults(func=cmd_linecmp)

    p = sub.add_parser(
        "localcmp", help="compare line tables of an object with the original"
    )
    p.add_argument("object")
    p.add_argument("function", nargs="?")
    p.add_argument("--unit")
    p.set_defaults(func=cmd_localcmp)

    args = parser.parse_args(argv)
    args.func(args)
    return 0


if __name__ == "__main__":
    sys.exit(main())
