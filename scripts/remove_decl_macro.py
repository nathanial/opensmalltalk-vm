#!/usr/bin/env python3
"""
Remove DECL_MAYBE_SQ_GLOBAL_STRUCT occurrences from generated sources.

The macro currently expands to nothing, so this script simply strips it from
the provided files (or recursively through directories) while tidying up any
trailing whitespace it leaves behind.
"""

from __future__ import annotations

import argparse
from pathlib import Path
from typing import Iterable, Tuple

TARGET = "DECL_MAYBE_SQ_GLOBAL_STRUCT"


def iter_sources(path: Path) -> Iterable[Path]:
    if path.is_file():
        yield path
        return
    for entry in path.rglob("*"):
        if entry.suffix.lower() in {".c", ".h", ".cc", ".cpp"} and entry.is_file():
            yield entry


def rewrite(path: Path, dry_run: bool) -> Tuple[int, bool]:
    text = path.read_text(encoding="utf-8")
    if TARGET not in text:
        return 0, False

    lines = text.splitlines(keepends=True)
    replacements = 0
    new_lines = []
    for line in lines:
        if TARGET not in line:
            new_lines.append(line)
            continue
        replacements += line.count(TARGET)
        updated = line.replace(TARGET, "")
        stripped = updated.rstrip()
        if stripped:
            new_lines.append(stripped + ("\n" if updated.endswith("\n") else ""))
        else:
            # preserve a blank line if there was one
            if updated.endswith("\n"):
                new_lines.append("\n")
    new_text = "".join(new_lines)
    if not dry_run:
        path.write_text(new_text, encoding="utf-8")
    return replacements, True


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "inputs",
        nargs="+",
        type=Path,
        help="Files or directories to process.",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Report changes without rewriting files.",
    )
    args = parser.parse_args()

    total_files = 0
    total_replacements = 0
    for input_path in args.inputs:
        resolved = input_path.resolve()
        if not resolved.exists():
            raise SystemExit(f"{resolved} does not exist.")
        for source_path in iter_sources(resolved):
            count, changed = rewrite(source_path, args.dry_run)
            if changed:
                total_files += 1
                total_replacements += count
                verb = "Would update" if args.dry_run else "Updated"
                print(f"{verb} {source_path}: {count} occurrence(s) removed")

    if total_files == 0:
        print("No DECL_MAYBE_SQ_GLOBAL_STRUCT occurrences found.")
    else:
        summary = (
            "Would remove" if args.dry_run else "Removed"
        )
        print(f"{summary} {total_replacements} macro occurrence(s) across {total_files} file(s).")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
