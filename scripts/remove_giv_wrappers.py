#!/usr/bin/env python3
"""
Strip GIV(...) macro wrappers from generated interpreter source files.

Given one or more files or directories, this script replaces every occurrence
of `GIV(name)` with `name`, assuming the macro simply aliases to the variable.
By default it rewrites C/C++ sources in place. Use `--dry-run` to preview
changes without writing.
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path
from typing import Iterable, List, Tuple

GIV_PATTERN = re.compile(r"\bGIV\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)")


def iter_source_files(path: Path) -> Iterable[Path]:
    """Yield candidate source files from `path`."""

    if path.is_file():
        yield path
        return

    for entry in path.rglob("*"):
        if entry.suffix.lower() in {".c", ".h", ".cc", ".cpp"} and entry.is_file():
            yield entry


def rewrite_file(path: Path, dry_run: bool) -> Tuple[int, bool]:
    """Apply the GIV replacement to `path`. Returns (count, changed?)."""

    text = path.read_text(encoding="utf-8")
    new_text, count = GIV_PATTERN.subn(r"\1", text)
    if count == 0:
        return 0, False

    if not dry_run:
        path.write_text(new_text, encoding="utf-8")
    return count, True


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "inputs",
        nargs="+",
        type=Path,
        help="Files or directories to process (directories searched recursively).",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Report replacements without modifying files.",
    )
    args = parser.parse_args()

    total_files = 0
    total_replacements = 0

    for input_path in args.inputs:
        resolved = input_path.resolve()
        if not resolved.exists():
            raise SystemExit(f"{resolved} does not exist.")
        for source_path in iter_source_files(resolved):
            count, changed = rewrite_file(source_path, args.dry_run)
            if changed:
                total_files += 1
                total_replacements += count
                print(
                    f"{'Would update' if args.dry_run else 'Updated'} {source_path}: "
                    f"{count} replacements"
                )

    if total_files == 0:
        print("No GIV(...) wrappers found.")
    else:
        action = "would replace" if args.dry_run else "replaced"
        print(
            f"{action.capitalize()} {total_replacements} occurrences across {total_files} file(s)."
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
