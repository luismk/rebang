#!/usr/bin/env python3
from __future__ import annotations

import argparse
import bisect
import difflib
import re
import struct
import sys
from collections import Counter
from dataclasses import dataclass
from pathlib import Path

import coffsym
import common
import original
import pdbinfo
from original import IMAGE_BASE

DIR32, DIR32NB, REL32 = 0x0006, 0x0007, 0x0014
Address = tuple[int, int]


def plain_name(symbol: str) -> str | None:
    # Clean up symbol name to try to match S_LPROC32 records
    if symbol.startswith("_$E"):
        return symbol[1:]
    c = re.fullmatch(r"[_@]([A-Za-z_$]\w*?)(?:@\d+)?", symbol)
    if c:
        return c[1]
    m = re.match(r"\?(\?[01])?([A-Za-z_]\w*)((?:@[A-Za-z_]\w*)*)@@", symbol)
    if not m:
        return None
    scopes = [s for s in m[3].split("@") if s][::-1]
    if m[1]:
        return "::".join([m[2], *scopes, ("~" if m[1] == "?1" else "") + m[2]])
    return "::".join([*scopes, m[2]])


@dataclass
class Result:
    section: coffsym.Section
    label: str
    status: str
    address: Address | None = None
    detail: str = ""
    ours: bytes = b""
    theirs: bytes = b""


class Comparison:
    def __init__(self, path: Path, module: original.Module | None):
        self.obj = coffsym.load(path)[0]
        self.module = module
        self.image = original.image()
        self.publics = pdbinfo.publics()
        self.located: dict[int, Address] = {}
        self.origin: dict[int, str] = {}
        self.results: dict[int, Result] = {}
        by_section: dict[int, list[coffsym.Symbol]] = {}
        for s in self.obj.symbols:
            if s.section > 0 and not s.name.startswith("."):
                by_section.setdefault(s.section, []).append(s)
        self.members = by_section
        self.statics: dict[str, list[tuple[Address, int]]] = {}
        if module is not None:
            for p in pdbinfo.module_symbols(module.number).procs:
                self.statics.setdefault(p.name, []).append(
                    ((p.section, p.offset), p.size)
                )
        for sec in self.obj.sections:
            if sec.name.startswith((".debug", ".drectve")) or sec.name == ".sxdata":
                continue
            self.locate_by_name(sec)

    def label(self, sec: coffsym.Section) -> str:
        symbols = sorted(
            self.members.get(sec.index, []),
            key=lambda s: (s.value, s.storage_class != 2),
        )
        return symbols[0].name if symbols else sec.name

    def locate_by_name(self, sec: coffsym.Section) -> None:
        for s in sorted(
            self.members.get(sec.index, []), key=lambda s: s.storage_class != 2
        ):
            address = self.publics.by_name.get(s.name) if s.storage_class == 2 else None
            origin = "public"
            if address is None:
                found = self.statics.get(plain_name(s.name) or "", [])
                sized = [a for a, size in found if size == sec.size] or [
                    a for a, _ in found
                ]
                address, origin = (sized[0] if sized else None), "module symbol"
            if address is not None:
                self.located[sec.index] = (address[0], address[1] - s.value)
                self.origin[sec.index] = origin
                return

    def owner(self, address: Address) -> original.Contribution | None:
        return original.contribution_at(*address)

    def owned(self, sec: coffsym.Section) -> bool:
        address = self.located.get(sec.index)
        if address is None or self.module is None:
            return False
        c = self.owner(address)
        return c is not None and c.module == self.module.number

    def relocations(
        self, sec: coffsym.Section
    ) -> list[tuple[int, coffsym.Symbol | None, int]]:
        out = []
        for i in range(sec.nrelocs):
            offset, index, kind = struct.unpack_from(
                "<IIH", self.obj.data, sec.rel_ptr + i * 10
            )
            out.append((offset, self.obj.symtab.get(index), kind))
        return out

    def target(self, symbol: coffsym.Symbol | None) -> Address | None:
        if symbol is None:
            return None
        if symbol.section > 0:
            base = self.located.get(symbol.section)
            return (base[0], base[1] + symbol.value) if base else None
        if symbol.section == 0:
            return self.publics.by_name.get(symbol.name)
        return None

    def compare(self, sec: coffsym.Section) -> Result:
        base = self.located[sec.index]
        data = (
            bytearray(self.obj.section_data(sec))
            if sec.raw_ptr
            else bytearray(sec.size)
        )
        theirs = bytearray(self.image.read(base[0], base[1], sec.size))
        pending = []
        for offset, symbol, kind in self.relocations(sec):
            if offset + 4 > len(data):
                continue
            addend = struct.unpack_from("<i", data, offset)[0]
            target = self.target(symbol)
            if target is None or kind not in (DIR32, DIR32NB, REL32):
                data[offset : offset + 4] = theirs[offset : offset + 4]
                if symbol is not None and symbol.section > 0 and kind in (DIR32, REL32):
                    pending.append((offset, symbol, kind, addend))
                continue
            if target[0] not in self.image.sections:
                if kind != DIR32:
                    data[offset : offset + 4] = theirs[offset : offset + 4]
                    continue
                struct.pack_into("<I", data, offset, (target[1] + addend) & 0xFFFFFFFF)
                continue
            rva = self.image.rva(*target) + addend
            if kind == DIR32:
                value = IMAGE_BASE + rva
            elif kind == DIR32NB:
                value = rva
            else:
                value = rva - (self.image.rva(base[0], base[1] + offset) + 4)
            struct.pack_into("<I", data, offset, value & 0xFFFFFFFF)
        status, detail = "MATCH", ""
        contribution = self.owner(base)
        if (
            contribution
            and contribution.offset == base[1]
            and contribution.size != sec.size
        ):
            status, detail = (
                "SIZE",
                f"ours {sec.size:#x}, original {contribution.size:#x}",
            )
        elif sec.raw_ptr and data != theirs:
            first = next(i for i in range(len(data)) if data[i] != theirs[i])
            status, detail = "DIFF", f"first difference at +{first:#x}"
        if status == "MATCH":
            for offset, symbol, kind, addend in pending:
                self.infer(sec, base, offset, symbol, kind, addend, theirs)
            names = self.publics.names_at(*base)
            ours = [
                m.name
                for m in self.members.get(sec.index, [])
                if m.storage_class == 2
                and m.value == 0
                and not original.PLACEHOLDER.match(m.name)
            ]
            if names and ours and not set(names) & set(ours):
                status, detail = (
                    "NAME",
                    f"match with different symbol name: {names[0]}",
                )
        return Result(
            sec, self.label(sec), status, base, detail, bytes(data), bytes(theirs)
        )

    def infer(self, sec, base, offset, symbol, kind, addend, theirs) -> None:
        value = struct.unpack_from("<I", theirs, offset)[0]
        if kind == DIR32:
            located = original.section_offset(value - addend)
        else:
            site = IMAGE_BASE + self.image.rva(base[0], base[1] + offset) + 4
            located = original.section_offset((site + value - addend) & 0xFFFFFFFF)
        if located is None:
            return
        child = (located[0], located[1] - symbol.value)
        if symbol.section not in self.located:
            self.located[symbol.section] = child
            self.origin[symbol.section] = f"via {self.label(sec)}"
        elif self.located[symbol.section] != child and symbol.section in self.results:
            result = self.results[symbol.section]
            result.status = "DIFF"
            result.detail = f"referenced at two addresses (also from {self.label(sec)})"

    def run(self) -> None:
        done: set[int] = set()
        while todo := sorted(i for i in self.located if i not in done):
            for i in todo:
                done.add(i)
                self.results[i] = self.compare(self.obj.sections[i - 1])
        if self.module is not None:
            self.locate_by_contribution()

    def locate_by_contribution(self) -> None:
        assert self.module is not None
        claimed = {a for a in self.located.values()}
        spare = [
            c
            for c in original.module_contributions().get(self.module.number, [])
            if (c.section, c.offset) not in claimed
        ]
        for sec in self.obj.sections:
            if sec.index in self.located or sec.name.startswith(
                (".debug", ".drectve", ".sxdata")
            ):
                continue
            for c in spare:
                if c.size != sec.size:
                    continue
                self.located[sec.index] = (c.section, c.offset)
                result = self.compare(sec)
                if result.status in ("MATCH", "NAME"):
                    self.origin[sec.index] = "by contents"
                    self.results[sec.index] = result
                    spare.remove(c)
                    break
                del self.located[sec.index]

    def owned_results(self) -> list[Result]:
        return [
            self.results[i]
            for i in sorted(self.results)
            if self.owned(self.obj.sections[i - 1])
        ]

    def associations(self) -> dict[int, int]:
        out = {}
        for s in self.obj.symbols:
            if (
                s.storage_class == 3
                and s.section > 0
                and s.name == self.obj.sections[s.section - 1].name
            ):
                sec = self.obj.sections[s.section - 1]
                if sec.selection == 5 and sec.sel_offset is not None:
                    out[s.section] = struct.unpack_from(
                        "<H", self.obj.data, sec.sel_offset - 2 - self.obj.base
                    )[0]
        return out

    def order(self) -> list[str]:
        groups: dict[str, list[Result]] = {}
        for r in self.owned_results():
            groups.setdefault(r.section.name, []).append(r)
        out = []
        for name, rows in groups.items():
            expected = [
                r.label for r in sorted(rows, key=lambda r: r.address or (0, 0))
            ]
            actual = [r.label for r in sorted(rows, key=lambda r: r.section.index)]
            matcher = difflib.SequenceMatcher(None, expected, actual, autojunk=False)
            for tag, i1, i2, j1, j2 in matcher.get_opcodes():
                if tag != "equal":
                    out.append(
                        f"{name} {tag}: original {expected[i1:i2]} / ours {actual[j1:j2]}"
                    )
        return out


def disassembly(ours: bytes, theirs: bytes) -> list[str]:
    def rows(data: bytes) -> list[tuple[int, bytes, str]]:
        return [
            (i.address, bytes(i.bytes), f"{i.mnemonic} {i.op_str}".strip())
            for i in coffsym.disassembler().disasm(data, 0)
        ]

    return coffsym.side_by_side(rows(ours), rows(theirs))


def compare_object(path: Path, unit: str | None, verbose: bool) -> int:
    module = original.find_module(unit or str(path))
    comparison = Comparison(path, module)
    comparison.run()
    results = comparison.owned_results()
    counts = Counter(r.status for r in results)
    ignored = (".debug", ".drectve", ".sxdata")
    associations = comparison.associations()
    sections = [s for s in comparison.obj.sections if not s.name.startswith(ignored)]
    elsewhere = [
        s for s in sections if s.index in comparison.located and not comparison.owned(s)
    ]
    unlocated = [s for s in sections if s.index not in comparison.located]
    foreign = {s.index for s in elsewhere}
    unlocated = [s for s in unlocated if associations.get(s.index) not in foreign]
    order = comparison.order()
    summary = ", ".join(f"{k} {v}" for k, v in sorted(counts.items()))
    shown = (
        path.resolve().relative_to(common.ROOT)
        if path.resolve().is_relative_to(common.ROOT)
        else path
    )
    print(f"{shown}: {module}")
    print(
        f"  {summary}; order {'DIFFERS' if order else 'ok'}; {len(elsewhere)} sections that "
        f"other modules own; {len(unlocated)} not located"
    )
    for r in results:
        if r.status != "MATCH" or verbose:
            where = f"{r.address[0]:04x}:{r.address[1]:08x}" if r.address else "?"
            print(
                f"  {r.status:5} {where} {r.section.name:9} {r.label[:100]} {r.detail}"
            )
            if verbose and r.status == "DIFF" and r.section.code:
                print(
                    "\n".join("      " + line for line in disassembly(r.ours, r.theirs))
                )
    for line in order[:10]:
        print(f"  ORDER {line}")
    for s in unlocated if verbose else ():
        print(f"  not located: {s.name} {comparison.label(s)[:100]}")
    return 1 if set(counts) - {"MATCH"} or order else 0


def cmd_obj(args: argparse.Namespace) -> int:
    status = 0
    for path in args.objects:
        status |= compare_object(Path(path), args.unit, args.verbose)
    return status


def cmd_all(args: argparse.Namespace) -> int:
    status = 0
    for unit in sorted(original.units(), key=lambda u: u.source):
        if not unit.converted:
            continue
        if not unit.product.exists():
            print(f"{unit.source}: not built")
            status = 1
            continue
        status |= compare_object(unit.product, str(unit.module.number), args.verbose)
    return status


def object_for(module: int) -> Path | None:
    unit = next((u for u in original.units() if u.module.number == module), None)
    return (
        unit.product
        if unit and unit.product.suffix == ".obj" and unit.product.exists()
        else None
    )


def candidates(
    comparison: Comparison, sec: coffsym.Section
) -> list[original.Contribution]:
    data = bytearray(comparison.obj.section_data(sec))
    offsets = [o for o, _, _ in comparison.relocations(sec)]
    for o in offsets:
        data[o : o + 4] = b"\0\0\0\0"
    out = []
    for c in original.contributions():
        if c.section != 1 or c.size != sec.size:
            continue
        theirs = bytearray(comparison.image.read(c.section, c.offset, c.size))
        for o in offsets:
            theirs[o : o + 4] = b"\0\0\0\0"
        if theirs == data:
            out.append(c)
    return out


def cmd_comdats(args: argparse.Namespace) -> int:
    path = Path(args.object)
    module = original.find_module(args.unit or str(path))
    comparison = Comparison(path, module)
    comparison.run()
    rank = original.link_rank(module.number)
    commands: list[str] = []
    rows: dict[str, list[str]] = {}
    loaded: dict[Path, dict[str, coffsym.Section | None]] = {}

    def defined(obj: Path) -> dict[str, coffsym.Section | None]:
        if obj not in loaded:
            m = coffsym.load(obj)[0]
            loaded[obj] = {
                s.name: (m.sections[s.section - 1] if s.section > 0 else None)
                for s in m.symbols
                if s.section > 0
            }
        return loaded[obj]

    for sec in comparison.obj.sections:
        if not sec.comdat or sec.selection == 5:
            continue
        name = comparison.label(sec)
        address = comparison.located.get(sec.index)
        if address is None:
            if sec.code:
                found = candidates(comparison, sec)
                text = ", ".join(
                    f"{c.placeholder} in {original.modules()[c.module].stem}"
                    + (" (links later)" if original.link_rank(c.module) > rank else "")
                    for c in found[:6]
                )
                rows.setdefault("NOPUB", []).append(
                    f"{name[:110]}\n      same bytes: {text or 'none'}"
                )
            continue
        via = (
            ""
            if comparison.origin.get(sec.index) == "public"
            else f", located {comparison.origin[sec.index]}"
        )
        c = comparison.owner(address)
        if c is None or c.module == module.number:
            continue
        result = comparison.results.get(sec.index)
        match = result.status if result else "?"
        owner = original.modules()[c.module]
        obj = object_for(c.module)
        where = f"{owner.stem} ({obj.relative_to(common.ROOT) if obj else 'no object'})"
        later = original.link_rank(c.module) > rank
        if obj is None:
            rows.setdefault("NO-OBJECT", []).append(f"{name[:110]} -> {where}")
            continue
        names = defined(obj)
        if name in names:
            owner_section = names[name]
            if owner_section is not None and owner_section.selection != 2:
                rows.setdefault("SELECTION", []).append(f"{name[:110]} in {where}")
                commands.append(
                    f"python tools/coffsym.py set-selection {obj.relative_to(common.ROOT)} '{name}' any"
                )
            elif args.verbose:
                rows.setdefault("OK", []).append(f"{name[:110]} ({match})")
        elif c.placeholder in names:
            if later:
                rows.setdefault("LATE-OWNER", []).append(
                    f"{name[:110]}\n      owner {where} links after this unit - original did not emit here"
                )
                continue
            rows.setdefault("RENAME", []).append(
                f"{c.placeholder} -> {name[:110]} in {where} ({match}{via})"
            )
            commands.append(f"python tools/coffsym.py rename {c.placeholder} '{name}'")
            commands.append(
                f"python tools/coffsym.py set-selection {obj.relative_to(common.ROOT)} '{name}' any"
            )
        else:
            rows.setdefault("MISSING", []).append(
                f"{name[:110]}: {where} defines neither it nor {c.placeholder}"
            )
        if match == "DIFF":
            rows.setdefault("DIFF", []).append(
                f"{name[:110]} differs from the copy in {where}"
            )
    notes = {
        "RENAME": "delinker placeholder is present earlier in link; rename it so it gets COMDAT folded",
        "SELECTION": "COMDAT symbol exists with name, but isn't marked 'any'",
        "LATE-OWNER": "we are emitting a symbol before it was emitted in the original binary",
        "NOPUB": "there is a match, but ours is public and the matches are not",
        "MISSING": "no symbol for this contribution",
        "DIFF": "differs from image copy",
        "NO-OBJECT": "module is not an object input",
    }
    for kind in (
        "RENAME",
        "SELECTION",
        "LATE-OWNER",
        "MISSING",
        "NOPUB",
        "DIFF",
        "NO-OBJECT",
        "OK",
    ):
        if kind in rows:
            print(f"{kind} ({len(rows[kind])}): {notes.get(kind, '')}")
            for row in rows[kind]:
                print(f"  {row}")
    if commands:
        print("\ncommands:")
        print("\n".join(commands))
    if not rows:
        print("nothing to reconcile")
    return 1 if commands or "LATE-OWNER" in rows or "MISSING" in rows else 0


MAP_LINE = re.compile(r"^\s*([0-9a-f]{4}):([0-9a-f]{8})\s+(\S+)\s+[0-9a-f]{8}\s")


def map_anchors(path: Path) -> list[tuple[int, int, int, str]]:
    publics = pdbinfo.publics()
    out = []
    text = path.read_text(errors="replace").split(" Static symbols", 1)[0]
    for line in text.splitlines():
        m = MAP_LINE.match(line)
        if not m:
            continue
        section, offset, name = int(m[1], 16), int(m[2], 16), m[3]
        c = original.placeholder_contribution(name)
        address = (c.section, c.offset) if c else publics.by_name.get(name)
        if address and address[0] == section:
            out.append((section, address[1], offset - address[1], name))
    return sorted(set(out))


def moved_symbols(linked: Path) -> list[tuple[int, int, int, str]]:
    map_path = linked.with_suffix("").with_suffix(".map")
    if not map_path.exists():
        return []
    moved, previous = [], {}
    for section, offset, delta, name in map_anchors(map_path):
        if delta != previous.get(section, 0):
            moved.append((section, offset, delta, name))
        previous[section] = delta
    return moved


def explain(linked: Path, limit: int = 30, out=sys.stdout) -> None:
    ours = bytearray(linked.read_bytes())
    for offset, value in common.IDENTITY:
        replacement = bytes.fromhex(value)
        ours[offset : offset + len(replacement)] = replacement
    image = original.image()
    theirs = image.data
    if len(ours) != len(theirs):
        print(f"size: ours {len(ours):#x}, theirs {len(theirs):#x}", file=out)
    ranges: list[list[int]] = []
    for i in range(min(len(ours), len(theirs))):
        if ours[i] != theirs[i]:
            if ranges and i - ranges[-1][1] <= 8:
                ranges[-1][1] = i
            else:
                ranges.append([i, i])
    print(
        f"{sum(e - s + 1 for s, e in ranges)} differing bytes in {len(ranges)} ranges",
        file=out,
    )
    if not ranges:
        return
    moved = moved_symbols(linked)
    if moved:
        print("symbols start to move at (section:offset, ours - theirs):", file=out)
        for section, offset, delta, name in moved[:12]:
            print(f"  {section:04x}:{offset:08x} {delta:+#x} {name[:100]}", file=out)
    names = sorted((a, n) for n, a in pdbinfo.publics().by_name.items())
    keys = [a for a, _ in names]
    located = [(start, end, image.locate(start)) for start, end in ranges]
    headers = sum(1 for _, _, where in located if where is None)
    if headers:
        print(f"  {headers} ranges in the PE headers", file=out)
    shown = [(s, e, w) for s, e, w in located if w is not None]
    for start, end, where in shown[:limit]:
        assert where is not None
        c = original.contribution_at(*where)
        i = bisect.bisect_right(keys, where) - 1
        near = (
            f"{names[i][1][:80]}+{where[1] - names[i][0][1]:#x}"
            if i >= 0 and names[i][0][0] == where[0]
            else ""
        )
        owner = original.modules()[c.module].stem if c else "-"
        print(
            f"  {start:#9x} ({end - start + 1:4}) {where[0]:04x}:{where[1]:08x} {owner:16} {near}",
            file=out,
        )
    if len(shown) > limit:
        print(f"  ... {len(shown) - limit} more ranges", file=out)


def cmd_diff(args: argparse.Namespace) -> int:
    linked = Path(args.linked)
    explain(linked, args.limit)
    return 0


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Image-level comparison tool")
    sub = parser.add_subparsers(dest="cmd", required=True)

    p = sub.add_parser("obj", help="compare objects with the original image")
    p.add_argument("objects", nargs="+")
    p.add_argument("--unit")
    p.add_argument("-v", "--verbose", action="store_true")
    p.set_defaults(func=cmd_obj)

    p = sub.add_parser("all")
    p.add_argument("-v", "--verbose", action="store_true")
    p.set_defaults(func=cmd_all)

    p = sub.add_parser("comdats", help="audit COMDATs")
    p.add_argument("object")
    p.add_argument("--unit")
    p.add_argument("-v", "--verbose", action="store_true")
    p.set_defaults(func=cmd_comdats)

    p = sub.add_parser("diff", help="try to explain why the image mismatches")
    p.add_argument(
        "linked",
        nargs="?",
        default=str(common.ROOT / "build/ProjectG_ReleaseQA.link.exe"),
    )
    p.add_argument("--limit", type=int, default=30)
    p.set_defaults(func=cmd_diff)

    args = parser.parse_args(argv)
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
