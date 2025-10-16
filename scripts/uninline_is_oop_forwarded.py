#!/usr/bin/env python3
"""
Replace hand-inlined `isOopForwarded` tests with calls to the shared helper.

The generated interpreter sources contain many occurrences of the pattern

    /* isOopForwarded: */
        ((!((EXPR & (tagMask())))) &&
         ((!((longAt((void *)(EXPR))) &
             ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))))

Regardless of whitespace layout, this script rewrites each instance as

    /* isOopForwarded: */
        isOopForwarded(EXPR)

Use `--dry-run` to preview replacements without modifying files.
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path
from typing import Iterable, List, Tuple

CLOSE_PARENS = r"(?:\s*\))+"
IS_OOP_FORWARDED_PATTERN = re.compile(
    r"/\* isOopForwarded: \*/"
    r"(?P<ws>\s*)"
    r"\(\(!\(\s*(?P<expr1>[^&]+?)\s*&\s*\(tagMask\(\)\)\s*"
    + CLOSE_PARENS
    + r"\s*&&\s*"
    r"\(\(!\(\(longAt\(\(\s*void \*\)\(\s*(?P<expr2>.*?)\s*\)\)\)\s*&\s*"
    r"\(\(classIndexMask\(\)\)\s*-\s*\(isForwardedObjectClassIndexPun\(\)\)\)\)\)"
    r"(?P<tail>"
    + CLOSE_PARENS
    + r")",
    re.MULTILINE | re.DOTALL,
)

COMMENT_FINDER = re.compile(r"/\* isOopForwarded: \*/")


def iter_source_files(path: Path) -> Iterable[Path]:
    """Yield candidate C-family source files rooted at `path`."""

    if path.is_file():
        yield path
        return

    for entry in path.rglob("*"):
        if entry.suffix.lower() in {".c", ".h", ".cc", ".cpp"} and entry.is_file():
            yield entry


def is_balanced(expr: str) -> bool:
    """Return True if parentheses in `expr` are balanced."""

    depth = 0
    for ch in expr:
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
            if depth < 0:
                return False
    return depth == 0


def strip_wrapping_parens(expr: str) -> str:
    """Remove redundant outer parentheses from `expr`."""

    expr = expr.strip()
    while expr.startswith("(") and expr.endswith(")"):
        inner = expr[1:-1].strip()
        if not inner:
            break
        if not is_balanced(inner):
            break
        expr = inner
    return expr


def normalize_expr(expr1: str, expr2: str) -> str:
    """Return the canonical expression shared by both matches."""

    norm1 = strip_wrapping_parens(expr1)
    norm2 = strip_wrapping_parens(expr2)
    if norm1 != norm2:
        raise ValueError(
            "Mismatched isOopForwarded expressions: "
            f"{expr1!r} vs {expr2!r}"
        )
    return norm1


def rewrite_file(path: Path, dry_run: bool) -> Tuple[int, bool]:
    """Apply the inlined `isOopForwarded` replacement to `path`."""

    text = path.read_text(encoding="utf-8")
    if not COMMENT_FINDER.search(text):
        return 0, False

    replacements: List[int] = []

    def _replace(match: re.Match[str]) -> str:
        replacements.append(1)
        ws = match.group("ws")
        expr1 = match.group("expr1")
        expr2 = match.group("expr2")
        tail = match.group("tail")
        prefix = match.group(0)[: match.start("tail") - match.start()]
        after_comment = prefix.split("*/", 1)[1]
        leading = after_comment.lstrip()
        lead_parens = 0
        for ch in leading:
            if ch == "(":
                lead_parens += 1
            else:
                break
        remove_closings = max(lead_parens - 1, 0)
        expr = normalize_expr(expr1, expr2)
        remaining = []
        skips = remove_closings
        for ch in tail:
            if ch == ")" and skips > 0:
                skips -= 1
                continue
            remaining.append(ch)
        new_tail = "".join(remaining)
        for idx, ch in enumerate(new_tail):
            if not ch.isspace():
                first_non_ws = ch
                break
        else:
            first_non_ws = None
        if first_non_ws != ")":
            new_tail = ")" + new_tail
        return f"/* isOopForwarded: */{ws}isOopForwarded({expr}){new_tail}"

    new_text, _ = IS_OOP_FORWARDED_PATTERN.subn(_replace, text)
    if not replacements:
        return 0, False

    if not dry_run:
        path.write_text(new_text, encoding="utf-8")
    return len(replacements), True


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

    total_replacements = 0
    changed_files = 0

    for input_path in args.inputs:
        resolved = input_path.resolve()
        if not resolved.exists():
            raise SystemExit(f"{resolved} does not exist.")
        for source_path in iter_source_files(resolved):
            count, changed = rewrite_file(source_path, args.dry_run)
            if changed:
                changed_files += 1
                total_replacements += count
                print(
                    f"{'Would update' if args.dry_run else 'Updated'} "
                    f"{source_path}: {count} replacement(s)"
                )

    if changed_files == 0:
        print("No inlined isOopForwarded checks found.")
    else:
        action = "would replace" if args.dry_run else "replaced"
        print(
            f"{action.capitalize()} {total_replacements} occurrence(s) across "
            f"{changed_files} file(s)."
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
