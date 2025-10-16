#!/usr/bin/env python3
"""
Remove redundant `/* isOopForwarded: */` comments from generated interpreter sources.

These comments marked former inline expansions of `isOopForwarded`. Now that the
code invokes the helper directly, the annotation is noise, so this script strips
both standalone comment lines and inline occurrences.
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path
from typing import Iterable, Tuple

COMMENT = "/* isOopForwarded: */"

COMMENT_LINE_RE = re.compile(r"^[ \t]*\/\* isOopForwarded: \*\/[ \t]*\n", re.MULTILINE)
COMMENT_INLINE_RE = re.compile(r"\/\* isOopForwarded: \*\/")


def iter_source_files(path: Path) -> Iterable[Path]:
    """Yield C-family sources rooted at `path`."""

    if path.is_file():
        yield path
        return

    for entry in path.rglob("*"):
        if entry.suffix.lower() in {".c", ".h", ".cc", ".cpp"} and entry.is_file():
            yield entry


def rewrite_file(path: Path, dry_run: bool) -> Tuple[int, bool]:
    """Strip `isOopForwarded` comments from `path`. Returns (count, changed?)."""

    text = path.read_text(encoding="utf-8")
    if COMMENT not in text:
        return 0, False

    line_matches = len(COMMENT_LINE_RE.findall(text))
    new_text = COMMENT_LINE_RE.sub("", text)
    new_text, inline_matches = COMMENT_INLINE_RE.subn("", new_text)
    total = line_matches + inline_matches
    if total == 0:
        return 0, False

    if not dry_run:
        path.write_text(new_text, encoding="utf-8")
    return total, True


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
        help="Report removals without modifying files.",
    )
    args = parser.parse_args()

    files_changed = 0
    total_removed = 0

    for input_path in args.inputs:
        resolved = input_path.resolve()
        if not resolved.exists():
            raise SystemExit(f"{resolved} does not exist.")
        for source_path in iter_source_files(resolved):
            removed, changed = rewrite_file(source_path, args.dry_run)
            if changed:
                files_changed += 1
                total_removed += removed
                print(
                    f"{'Would update' if args.dry_run else 'Updated'} "
                    f"{source_path}: removed {removed} comment(s)"
                )

    if files_changed == 0:
        print("No /* isOopForwarded: */ comments found.")
    else:
        action = "would remove" if args.dry_run else "removed"
        print(
            f"{action.capitalize()} {total_removed} comment(s) across "
            f"{files_changed} file(s)."
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
