#!/usr/bin/env python3
"""
Compact consecutive single-variable declarations that share the same type.

Example:
    sqInt count;
    sqInt delta;
    sqInt limit;

turns into:
    sqInt count, delta, limit;

Only plain declarations without initializers are merged, and only when they
appear consecutively (with no intervening non-blank lines) at the same
indentation level.
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path
from typing import List, Tuple

DECL_RE = re.compile(
    r"""
    ^(?P<indent>\s*)                 # leading whitespace
    (?P<type>[A-Za-z_][\w\s\*]*?)    # type (with optional pointer stars/spaces)
    \s+
    (?P<name>[A-Za-z_]\w*)           # variable name
    \s*;
    \s*$
    """,
    re.VERBOSE,
)


def compact_decls(text: str) -> Tuple[str, int]:
    lines = text.splitlines(keepends=True)
    result: List[str] = []
    i = 0
    replacements = 0

    while i < len(lines):
        match = DECL_RE.match(lines[i])
        if not match:
            result.append(lines[i])
            i += 1
            continue

        indent = match.group("indent")
        decl_type = match.group("type")
        names = [match.group("name")]
        j = i + 1

        while j < len(lines):
            next_line = lines[j]
            if not next_line.strip():
                break
            next_match = DECL_RE.match(next_line)
            if (
                not next_match
                or next_match.group("indent") != indent
                or next_match.group("type") != decl_type
            ):
                break
            names.append(next_match.group("name"))
            j += 1

        if len(names) > 1:
            line_end = "\n" if lines[j - 1].endswith("\n") else ""
            merged = f"{indent}{decl_type} {', '.join(names)};{line_end}"
            result.append(merged)
            replacements += len(names) - 1
            i = j
        else:
            result.append(lines[i])
            i += 1

    return "".join(result), replacements


def process_file(path: Path, dry_run: bool) -> int:
    original = path.read_text(encoding="utf-8")
    updated, replacements = compact_decls(original)
    if replacements == 0:
        return 0
    if not dry_run:
        path.write_text(updated, encoding="utf-8")
    return replacements


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path, help="File to rewrite.")
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Report changes without modifying the file.",
    )
    args = parser.parse_args()

    replacements = process_file(args.source.resolve(), args.dry_run)
    if replacements == 0:
        print("No declaration groups found.")
    else:
        if args.dry_run:
            print(f"Would merge {replacements} declaration(s).")
        else:
            print(f"Merged {replacements} declaration(s).")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
