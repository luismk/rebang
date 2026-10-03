#!/usr/bin/env python3
from __future__ import annotations

import argparse
import concurrent.futures
import difflib
import io
import os
import runpy
import shutil
import sys
from collections import Counter
from contextlib import redirect_stdout
from dataclasses import dataclass
from pathlib import Path

import coffsym
import common
import imgcmp
import original
import pdbinfo

WORK = common.ROOT / "build/probe"
MIRRORED = ("source", "tools")
Edit = tuple[str, str] | tuple[str, str, str]


@dataclass
class Setup:
    unit: original.Unit
    source: Path
    flags: list[str]
    includes: list[str]
    pch: str | None
    pch_flags: list[str]
    pch_changed: bool
    work: Path


@dataclass
class Report:
    counts: Counter
    rows: list[imgcmp.Result]
    order: list[str]
    missing: list[original.Contribution]
    extra: list[str]
    distance: int

    @property
    def summary(self) -> str:
        parts = [f"{k} {v}" for k, v in sorted(self.counts.items())]
        if self.distance:
            parts.append(f"{self.distance} instructions differ")
        parts.append("order DIFFERS" if self.order else "order ok")
        if self.missing:
            parts.append(f"{len(self.missing)} original contributions missing")
        if self.extra:
            parts.append(f"{len(self.extra)} of ours not located")
        return ", ".join(parts)


def make_setup(args: argparse.Namespace) -> Setup:
    unit = original.find_unit(args.unit)
    flags, includes, pch = common.target().compile_settings(unit.source)
    pch = args.pch or pch
    pch_flags = common.target().compile_settings(pch)[0] if pch else []

    def adjust(flags: list[str]) -> list[str]:
        flags = flags + (args.add_flags or "").split()
        return [f for f in flags if f not in (args.remove_flags or "").split()]

    if args.flags:
        flags = args.flags.split()
        pch_flags = [f for f in flags if not f.startswith(("/FI", "/Yc", "/Yu"))]
    changed = bool(args.flags or args.add_flags or args.remove_flags)
    source = Path(args.source).resolve() if args.source else common.ROOT / unit.source
    work = WORK / unit.module.stem
    return Setup(
        unit, source, adjust(flags), includes, pch, adjust(pch_flags), changed, work
    )


def windows(path: Path | str) -> str:
    return "Z:" + os.path.abspath(path).replace("/", "\\")


def mirror(root: Path, edits: dict[str, str]) -> None:
    for top in MIRRORED:
        (root / top).mkdir(parents=True, exist_ok=True)
        for entry in (common.ROOT / top).iterdir():
            (root / top / entry.name).symlink_to(entry)
    for relative, text in edits.items():
        path = root / relative
        for parent in reversed(Path(relative).parents):
            here = root / parent
            if here.is_symlink():
                real = here.resolve()
                here.unlink()
                here.mkdir()
                for entry in real.iterdir():
                    (here / entry.name).symlink_to(entry)
        path.unlink(missing_ok=True)
        path.write_text(text, encoding="latin1")


def pch_is_current(pch_source: str) -> bool:
    pch = common.ROOT / common.target().pch_product(pch_source)
    depfile = Path(str(pch) + ".d")
    if not pch.exists() or not depfile.exists():
        return False
    deps = depfile.read_text().split(":", 1)[1].split("\n", 1)[0].split()
    newest = max(
        (common.ROOT / d).stat().st_mtime
        for d in [pch_source, *deps]
        if (common.ROOT / d).exists()
    )
    return pch.stat().st_mtime >= newest


def compile_pch(s: Setup, work: Path, includes: list[str], source: Path) -> None:
    assert s.pch is not None
    stem = Path(s.pch).stem
    common.invoke(
        "cl.exe",
        work / f"{stem}.pch.obj",
        [
            "/nologo",
            "/c",
            *s.pch_flags,
            f"/Yc{stem}.h",
            "/Fp" + windows(work / f"{stem}.pch"),
            *["/I" + windows(i) for i in includes],
            "/Fo" + windows(work / f"{stem}.pch.obj"),
            "/Fd" + windows(work / f"{stem}.pdb"),
            windows(source),
        ],
    )


def compile_variant(s: Setup, name: str, edits: list[Edit]) -> tuple[Path | None, str]:
    work = s.work / name
    shutil.rmtree(work, ignore_errors=True)
    work.mkdir(parents=True)
    source_rel = (
        str(s.source.relative_to(common.ROOT))
        if s.source.is_relative_to(common.ROOT)
        else None
    )
    text = s.source.read_text(encoding="latin1")
    files: dict[str, str] = {}
    for edit in edits:
        path, old, new = edit if len(edit) == 3 else (None, *edit)
        if path is None or path == source_rel:
            if old not in text:
                return None, f"source edit not found: {old[:80]!r}"
            text = text.replace(old, new, 1)
            continue
        current = files.get(path) or (common.ROOT / path).read_text(encoding="latin1")
        if old not in current:
            return None, f"{path}: edit not found: {old[:80]!r}"
        files[path] = current.replace(old, new, 1)
    directories = [s.unit.source.rsplit("/", 1)[0], *s.includes]
    if s.pch and s.pch.rsplit("/", 1)[0] not in directories:
        directories.append(s.pch.rsplit("/", 1)[0])
    if files:
        if outside := [
            f for f in files if not f.startswith(tuple(f"{m}/" for m in MIRRORED))
        ]:
            return (
                None,
                f"only files under {', '.join(MIRRORED)} can be edited: {outside}",
            )
        mirror(work / "mirror", files)
        includes = [str(work / "mirror" / d) for d in directories]
    else:
        includes = [str(common.ROOT / d) for d in directories]
    compiled = work / s.source.name
    compiled.write_text(text, encoding="latin1")
    obj = work / (s.source.stem + ".obj")
    pch_flags: list[str] = []
    try:
        if s.pch:
            stem = Path(s.pch).stem
            if files or s.pch_changed or not pch_is_current(s.pch):
                pch_source = (work / "mirror" / s.pch) if files else common.ROOT / s.pch
                compile_pch(s, work, includes, pch_source)
            else:
                product = common.ROOT / common.target().pch_product(s.pch)
                shutil.copy(product, work / f"{stem}.pch")
                if product.with_suffix(".pdb").exists():
                    shutil.copy(product.with_suffix(".pdb"), work / f"{stem}.pdb")
            pch_flags = [f"/Yu{stem}.h", "/Fp" + windows(work / f"{stem}.pch")]
        pdb = work / (f"{Path(s.pch).stem}.pdb" if s.pch else f"{s.source.stem}.pdb")
        log = common.invoke(
            "cl.exe",
            obj,
            [
                "/nologo",
                "/c",
                *s.flags,
                *pch_flags,
                *["/I" + windows(i) for i in includes],
                "/Fo" + windows(obj),
                "/Fd" + windows(pdb),
                windows(compiled),
            ],
        )
    except RuntimeError as error:
        return None, str(error)
    if not obj.exists():
        return (
            None,
            "no object emitted\n" + log,
        )
    warnings = [
        line for line in log.splitlines() if " warning " in line or " error " in line
    ]
    return obj, "\n".join(warnings)


def instruction_diff(result: imgcmp.Result) -> int:
    def text(data: bytes) -> list[str]:
        return [
            f"{i.mnemonic} {i.op_str}" for i in coffsym.disassembler().disasm(data, 0)
        ]

    a, b = text(result.ours), text(result.theirs)
    matcher = difflib.SequenceMatcher(None, a, b, autojunk=False)
    return sum(
        max(i2 - i1, j2 - j1)
        for tag, i1, i2, j1, j2 in matcher.get_opcodes()
        if tag != "equal"
    )


def evaluate(obj: Path, module: original.Module) -> Report:
    comparison = imgcmp.Comparison(obj, module)
    comparison.run()
    rows = comparison.owned_results()
    claimed = {r.address for r in rows}
    missing = [
        c
        for c in original.module_contributions().get(module.number, [])
        if (c.section, c.offset) not in claimed and c.size
    ]
    associations = comparison.associations()
    foreign = {
        i
        for i in comparison.located
        if not comparison.owned(comparison.obj.sections[i - 1])
    }
    extra = [
        comparison.label(s)
        for s in comparison.obj.sections
        if s.index not in comparison.located
        and not s.name.startswith((".debug", ".drectve", ".sxdata"))
        and associations.get(s.index) not in foreign
    ]
    distance = sum(
        instruction_diff(r)
        for r in rows
        if r.status in ("DIFF", "SIZE") and r.section.code
    )
    return Report(
        Counter(r.status for r in rows),
        rows,
        comparison.order(),
        missing,
        extra,
        distance,
    )


def print_report(report: Report, key: str | None) -> None:
    print(report.summary)
    names = pdbinfo.publics()
    for r in report.rows:
        if r.status == "MATCH":
            continue
        where = f"{r.address[0]:04x}:{r.address[1]:08x}" if r.address else "?"
        n = f" n={instruction_diff(r)}" if r.section.code else ""
        print(f"  {r.status:5} {where} {r.label[:110]}{n} {r.detail}")
        if key and (key == "*" or key in r.label) and r.section.code:
            print(
                "\n".join(
                    "      " + line for line in imgcmp.disassembly(r.ours, r.theirs)
                )
            )
    for line in report.order[:10]:
        print(f"  ORDER {line}")
    for c in report.missing[:20]:
        label = ", ".join(names.names_at(c.section, c.offset)) or c.placeholder
        print(f"  MISSING {c} {label[:110]}")
    for name in report.extra[:20]:
        print(f"  EXTRA {name[:110]}")


def load_variants(path: str) -> list[tuple[str, list[Edit]]]:
    return runpy.run_path(path)["VARIANTS"]


def run_variants(s: Setup, args: argparse.Namespace) -> int:
    variants = [("base", [])] + load_variants(args.variants)

    def one(variant: tuple[str, list[Edit]]) -> tuple[str, str]:
        name, edits = variant
        obj, log = compile_variant(s, name, edits)
        if obj is None:
            return name, f"== {name}: FAILED {log.splitlines()[0] if log else ''}"
        report = evaluate(obj, s.unit.module)
        lines = [f"== {name}: {report.summary}"]
        for r in report.rows:
            if args.only and args.only in r.label:
                n = (
                    f" n={instruction_diff(r)}"
                    if r.status != "MATCH" and r.section.code
                    else ""
                )
                lines.append(f"    {r.status:5} {r.label[:110]}{n}")
        if args.show == name:
            buffer = io.StringIO()
            with redirect_stdout(buffer):
                print_report(report, args.diff)
            lines.append(buffer.getvalue())
        return name, "\n".join(lines)

    with concurrent.futures.ThreadPoolExecutor(args.jobs) as pool:
        for _, text in pool.map(one, variants):
            print(text, flush=True)
    return 0


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Compile a unit and compare it with the original"
    )
    parser.add_argument("unit")
    parser.add_argument("source", nargs="?")
    parser.add_argument(
        "-d",
        "--diff",
        metavar="KEY",
    )
    parser.add_argument("--flags")
    parser.add_argument("--add-flags")
    parser.add_argument("--remove-flags")
    parser.add_argument("--pch")
    parser.add_argument("--variants", metavar="SPEC")
    parser.add_argument("--only", metavar="KEY")
    parser.add_argument("--show", metavar="NAME")
    # Wine overhead is a little heavy so go cores / 2...
    parser.add_argument(
        "-j", "--jobs", type=int, default=max(1, (os.cpu_count() or 2) // 2)
    )
    args = parser.parse_args(argv)
    s = make_setup(args)
    if args.variants:
        return run_variants(s, args)
    if s.source.suffix.lower() == ".obj":
        obj = s.source
    else:
        compiled, log = compile_variant(s, "base", [])
        if log:
            print(log)
        if compiled is None:
            return 1
        obj = compiled

    report = evaluate(obj, s.unit.module)
    print_report(report, args.diff)

    if set(report.counts) - {"MATCH"} or report.order or report.missing:
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
