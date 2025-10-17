#!/usr/bin/env python3
"""
Collapse blank lines between consecutive '#include' directives.

Example transformation:

    #include "a.h"


    #include "b.h"

becomes:

    #include "a.h"
    #include "b.h"

Usage:
    ./scripts/remove_include_blank_lines.py path/to/file.c [-o output.c] [--dry-run]
"""

from __future__ import annotations

import argparse
from pathlib import Path
from typing import List, Optional


def next_nonblank(lines: List[str], start: int) -> Optional[str]:
    """Return the next non-blank line text after index `start`, if any."""
    for line in lines[start + 1 :]:
        if line.strip():
            return line
    return None


def collapse_blank_lines(source_text: str) -> str:
    """Return `source_text` with blank lines removed between include blocks."""

    lines = source_text.splitlines(keepends=True)
    result: List[str] = []
    prev_nonblank_is_include = False

    for idx, line in enumerate(lines):
        stripped = line.strip()
        if not stripped:
            upcoming = next_nonblank(lines, idx)
            if (
                prev_nonblank_is_include
                and upcoming is not None
                and upcoming.lstrip().startswith("#include")
            ):
                continue
            result.append(line)
            continue

        is_include = line.lstrip().startswith("#include")
        result.append(line)
        prev_nonblank_is_include = is_include

    return "".join(result)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path, help="File to rewrite.")
    parser.add_argument(
        "-o",
        "--output",
        type=Path,
        help="Destination path. Defaults to rewriting SOURCE in place.",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Report whether changes are needed without writing output.",
    )
    args = parser.parse_args()

    source_path = args.source.resolve()
    if not source_path.exists():
        raise SystemExit(f"{source_path} does not exist.")

    original = source_path.read_text(encoding="utf-8")
    updated = collapse_blank_lines(original)

    if updated == original:
        print("No blank include gaps detected; no changes made.")
        return 0

    if args.dry_run:
        print("Blank include gaps detected; dry-run enabled, skipping write.")
        return 0

    output_path = args.output.resolve() if args.output else source_path
    output_path.write_text(updated, encoding="utf-8")
    print(f"Wrote updated file to {output_path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
