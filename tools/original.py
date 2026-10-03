from __future__ import annotations

import bisect
import csv
import functools
import re
import struct
from dataclasses import dataclass
from pathlib import Path

import common

IMAGE_BASE = 0x00400000
PLACEHOLDER = re.compile(r"__pg_c_([0-9a-f]{6})$")
UNIT_SUFFIXES = (".obj", ".c", ".cpp", ".cxx")
SECTION_NAMES = {
    1: ".text",
    2: ".rdata",
    3: ".data",
    4: ".tls",
    5: ".rsrc",
    6: ".reloc",
}


@dataclass(frozen=True)
class Contribution:
    id: int
    module: int
    section: int
    offset: int
    size: int

    @property
    def placeholder(self) -> str:
        return f"__pg_c_{self.id:06x}"

    @property
    def end(self) -> int:
        return self.offset + self.size

    def __str__(self) -> str:
        return f"{self.section:04x}:{self.offset:08x}+{self.size:#x}"


@dataclass(frozen=True)
class Module:
    number: int
    original_object: str
    original_module: str
    path: str | None

    @property
    def stem(self) -> str:
        return re.split(r"[\\/]", self.original_object)[-1].rpartition(".")[0].lower()

    @property
    def cvdump(self) -> int:
        return self.number + 1

    def __str__(self) -> str:
        return f"module {self.number} ({self.original_object})"


def read_csv(name: str) -> list[dict[str, str]]:
    with (common.DOCS / name).open(newline="") as f:
        return list(csv.DictReader(f))


@functools.cache
def contributions() -> list[Contribution]:
    return [
        Contribution(
            int(r["contribution"]),
            int(r["module"]),
            int(r["section"]),
            int(r["offset"], 16),
            int(r["size"], 16),
        )
        for r in read_csv("object-boundaries.csv")
    ]


@functools.cache
def _by_section() -> dict[int, tuple[list[int], list[Contribution]]]:
    out: dict[int, tuple[list[int], list[Contribution]]] = {}
    for c in sorted(contributions(), key=lambda c: (c.section, c.offset)):
        offsets, rows = out.setdefault(c.section, ([], []))
        offsets.append(c.offset)
        rows.append(c)
    return out


def contribution_at(section: int, offset: int) -> Contribution | None:
    offsets, rows = _by_section().get(section, ([], []))
    i = bisect.bisect_right(offsets, offset) - 1
    if i >= 0 and rows[i].offset <= offset < rows[i].end:
        return rows[i]
    return None


def placeholder_contribution(name: str) -> Contribution | None:
    m = PLACEHOLDER.match(name)
    return contributions()[int(m[1], 16)] if m else None


@functools.cache
def module_contributions() -> dict[int, list[Contribution]]:
    out: dict[int, list[Contribution]] = {}
    for c in contributions():
        out.setdefault(c.module, []).append(c)
    return out


@functools.cache
def modules() -> dict[int, Module]:
    paths = {int(r["module"]): r["path"] for r in read_csv("module-sizes.csv")}
    return {
        int(r["module"]): Module(
            int(r["module"]),
            r["original_object"],
            r["original_module"],
            paths.get(int(r["module"])),
        )
        for r in read_csv("module-list.csv")
    }


def find_module(key: str) -> Module:
    table = modules()
    if key.isdigit():
        return table[int(key)]
    path = Path(key.replace("\\", "/"))
    if len(path.parts) > 1:
        if path.is_absolute() and path.is_relative_to(common.ROOT):
            path = path.relative_to(common.ROOT)
        if path.is_relative_to(common.target().build):
            path = path.relative_to(common.target().build)
        wanted = str(path.with_suffix("")).lower()
        for m in table.values():
            if m.path and str(Path(m.path).with_suffix("")).lower() == wanted:
                return m
    stem = path.name.rpartition(".")[0] or path.name
    found = [m for m in table.values() if m.stem == stem.lower()]
    if len(found) == 1:
        return found[0]
    if not found:
        raise SystemExit(f"no module matches {key!r}")
    raise SystemExit(
        f"{key!r} is ambiguous: "
        + ", ".join(f"{m.number} {m.original_object}" for m in found)
    )


def section_offset(va: int) -> tuple[int, int] | None:
    rva = va - IMAGE_BASE
    for r in read_csv("module-sections.csv"):
        start, size = int(r["rva"], 16), int(r["virtual_size"], 16)
        if start <= rva < start + size:
            return int(r["section_id"]), rva - start
    return None


def link_rank(module: int) -> int:
    text = [c.offset for c in module_contributions().get(module, []) if c.section == 1]
    return min(text) if text else 1 << 32


@dataclass(frozen=True)
class Unit:
    source: str
    module: Module
    options: dict

    @property
    def converted(self) -> bool:
        return Path(self.source).suffix.lower() != ".obj"

    @property
    def product(self) -> Path:
        if self.converted:
            return common.ROOT / common.target().product(self.source)
        return common.ROOT / self.source


def _entries() -> tuple[dict[str, dict], set[str]]:
    config, entries = common.target().settings()
    pch = {common.entry_path(e) for e in config.get("precompiled_headers", [])}
    options: dict[str, dict] = {
        k: dict(v) if isinstance(v, dict) else {"path": v} for k, v in entries.items()
    }
    return options, pch


@functools.cache
def units() -> list[Unit]:
    entries, pch = _entries()
    by_path = {
        str(Path(m.path).with_suffix("")).lower(): m
        for m in modules().values()
        if m.path
    }
    out, rest = [], []
    for source, options in entries.items():
        if (
            "member" in options
            or source in pch
            or Path(source).suffix.lower() not in UNIT_SUFFIXES
        ):
            continue
        module = by_path.get(str(Path(source).with_suffix("")).lower())
        if module is None:
            rest.append((source, options))
        else:
            out.append(Unit(source, module, options))
    claimed = {u.module.number for u in out}
    by_stem: dict[str, list[Module]] = {}
    for m in modules().values():
        if m.number not in claimed:
            by_stem.setdefault(m.stem, []).append(m)
    for source, options in rest:
        found = by_stem.get(Path(source).stem.lower(), [])
        if len(found) == 1:
            out.append(Unit(source, found[0], options))
    return out


def find_unit(key: str) -> Unit:
    module = find_module(key)
    for unit in units():
        if unit.module.number == module.number:
            return unit
    raise SystemExit(f"{module} not found in {common.target().config}")


class Image:
    def __init__(self, path: Path):
        self.path = path
        self.data = self.path.read_bytes()
        pe = struct.unpack_from("<I", self.data, 0x3C)[0]
        count = struct.unpack_from("<H", self.data, pe + 6)[0]
        optional = struct.unpack_from("<H", self.data, pe + 20)[0]
        self.sections: dict[int, tuple[str, int, int, int, int]] = {}
        for i in range(count):
            o = pe + 24 + optional + i * 40
            name = self.data[o : o + 8].rstrip(b"\0").decode()
            vsize, rva, raw_size, raw = struct.unpack_from("<IIII", self.data, o + 8)
            self.sections[i + 1] = (name, rva, vsize, raw, raw_size)

    def rva(self, section: int, offset: int) -> int:
        return self.sections[section][1] + offset

    def read(self, section: int, offset: int, size: int) -> bytes:
        _, _, _, raw, raw_size = self.sections[section]
        avail = max(0, min(size, raw_size - offset))
        chunk = self.data[raw + offset : raw + offset + avail]
        return chunk + bytes(size - len(chunk))

    def locate(self, file_offset: int) -> tuple[int, int] | None:
        for number, (_, _, vsize, raw, raw_size) in self.sections.items():
            if raw <= file_offset < raw + min(vsize, raw_size):
                return number, file_offset - raw
        return None


@functools.cache
def image() -> Image:
    return Image(common.target().original)
