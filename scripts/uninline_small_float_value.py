#!/usr/bin/env python3
"""
Replace hand-inlined smallFloatValueOf: expansions with calls to
smallFloatValueOf().

The translated Spur sources often inline the immediate-float decoding as:

    /* begin smallFloatValueOf: */
    /* begin smallFloatBitsOf: */
    assert(isImmediateFloat(expr));
    bits = ((((usqInt)expr))) >> (numTagBits());
    if (bits > 1) {
        bits += (((usqInt)((smallFloatExponentOffset()))
                  << ((smallFloatMantissaBits()) + 1)));
    }
    /* a.k.a. ~= +/-0.0 */
    bits = ((bits << 0x3F)) + (((((usqInt)bits))) >> 1);
    memcpy((&value), (&bits), sizeof(value));

This script collapses the block to `value = smallFloatValueOf(expr);`,
preserving indentation and leaving subsequent uses of `value` untouched.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Iterable

BLOCK_RE = re.compile(
    r"(?P<indent>^[ \t]*)/\* begin smallFloatValueOf: \*/\s*"
    r"(?P=indent)/\* begin smallFloatBitsOf: \*/\s*"
    r"(?P<body>.*?)"
    r"(?P=indent)memcpy\(\s*\(&(?P<dest>[A-Za-z_][A-Za-z0-9_]*)\)\s*,\s*\(&bits\)\s*,\s*sizeof\(\s*(?P<size>[^)]+)\)\s*\)\s*;"
    r"(?P<tail>(?:\s*\r?\n[ \t]*)*)",
    re.DOTALL | re.MULTILINE,
)


def extract_expr(body: str) -> str | None:
    match = re.search(
        r"assert\(\s*isImmediateFloat\((?P<expr>.*?)\)\s*\);",
        body,
        re.DOTALL,
    )
    if not match:
        return None
    expr = match.group("expr").strip()
    # Clean up redundant parentheses introduced by the generator.
    while expr.startswith("(") and expr.endswith(")"):
        inner = expr[1:-1].strip()
        if not inner:
            break
        # Stop if removing parentheses would unbalance nested ones.
        depth = 0
        balanced = True
        for ch in inner:
            if ch == "(":
                depth += 1
            elif ch == ")":
                depth -= 1
                if depth < 0:
                    balanced = False
                    break
        if not balanced or depth != 0:
            break
        expr = inner
    return expr


def replace_block(match: re.Match[str]) -> str:
    body = match.group("body")
    expr = extract_expr(body)
    if not expr:
        return match.group(0)

    indent = match.group("indent")
    dest = match.group("dest")
    tail = match.group("tail") or "\n"

    # Ensure tail preserves at least a single newline.
    if not tail.startswith(("\n", "\r")):
        tail = "\n" + tail

    return f"{indent}{dest} = smallFloatValueOf({expr});{tail}"


def process_text(text: str) -> tuple[str, bool]:
    new_text, count = BLOCK_RE.subn(replace_block, text)
    return new_text, count > 0


def iter_source_files(paths: Iterable[Path]) -> Iterable[Path]:
    for path in paths:
        if path.is_dir():
            yield from (
                p
                for p in path.rglob("*.c")
                if p.name != "smallFloatValueOf.c"
            )
        elif path.suffix == ".c" and path.name != "smallFloatValueOf.c":
            yield path


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Rewrite inline smallFloatValueOf expansions."
    )
    parser.add_argument("paths", nargs="+", help="Files or directories to process.")
    args = parser.parse_args(argv)

    any_changes = False
    for source in iter_source_files(Path(p) for p in args.paths):
        original = source.read_text()
        updated, changed = process_text(original)
        if changed:
            source.write_text(updated)
            print(f"Updated {source}", file=sys.stderr)
            any_changes = True
    return 0 if any_changes else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
