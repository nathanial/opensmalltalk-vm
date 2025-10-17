#!/usr/bin/env python3
"""
Replace hand-inlined `isOopCompiledMethod:` predicate expansions with calls to
`isOopCompiledMethod(variable)`.

The generated sources expand the helper as:

    /* isOopCompiledMethod: */
         ((!(foo & (tagMask())))) &&
         (((byteAt((void *)(foo + (formatFieldByteOffset())))) &
           (formatMask())) >= (firstCompiledMethodFormat())))

This script finds each occurrence (via the comment marker), extracts the
enclosed expression, and substitutes the concise helper call while preserving
surrounding parentheses/formatting.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Iterable, Tuple

MARKER = "/* isOopCompiledMethod: */"
TAIL_SENTINEL = "firstCompiledMethodFormat())))"
VAR_PATTERN = re.compile(
    r"\(\s*!\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*&\s*\(tagMask\(\)\)\s*\)"
)


def rewrite(text: str, path: Path) -> Tuple[str, int]:
    """Replace inline expansions and return (rewritten_text, replacement_count)."""

    index = 0
    replacements = 0
    pieces = []

    while True:
        marker_index = text.find(MARKER, index)
        if marker_index == -1:
            pieces.append(text[index:])
            break

        pieces.append(text[index:marker_index])
        sentinel_index = text.find(TAIL_SENTINEL, marker_index)
        if sentinel_index == -1:
            raise RuntimeError(
                f"Unable to locate closing sentinel for inline isOopCompiledMethod in {path}"
            )
        tail_end = sentinel_index + len(TAIL_SENTINEL)
        segment = text[marker_index:tail_end]

        match = VAR_PATTERN.search(segment)
        if not match:
            raise RuntimeError(
                f"Unable to identify variable in inline isOopCompiledMethod block within {path}"
            )
        var = match.group(1)

        opens = segment.count("(")
        closes = segment.count(")")
        extra_closing = max(0, closes - opens)
        replacement = f"isOopCompiledMethod({var})" + (")" * extra_closing)

        pieces.append(replacement)
        index = tail_end
        replacements += 1

    rewritten = "".join(pieces)
    if MARKER in rewritten:
        raise RuntimeError(
            f"Residual isOopCompiledMethod marker found in {path} after rewriting"
        )
    return rewritten, replacements


def iter_targets(paths: Iterable[Path]) -> Iterable[Path]:
    for path in paths:
        if path.is_dir():
            yield from sorted(p for p in path.rglob("*.c") if p.is_file())
        else:
            yield path


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Collapse inline isOopCompiledMethod expansions to helper calls."
    )
    parser.add_argument("paths", nargs="+", type=Path, help="Files or directories to process.")
    args = parser.parse_args(argv)

    total = 0
    for target in iter_targets(args.paths):
        original = target.read_text()
        rewritten, count = rewrite(original, target)
        if count:
            target.write_text(rewritten)
            print(
                f"{target}: replaced {count} inline isOopCompiledMethod occurrence"
                f"{'' if count == 1 else 's'}"
            )
            total += count
    return 0 if total >= 0 else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
