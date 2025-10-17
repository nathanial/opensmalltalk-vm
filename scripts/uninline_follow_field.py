#!/usr/bin/env python3
"""
Replace hand-inlined followField:ofObject: sequences with calls to
followFieldofObject(fieldIndex, object).

Many generated interpreter sources expand the helper as:

    /* begin followField:ofObject: */
    target = fetchPointerofObject(fieldExpr, objectExpr);
    if (isOopForwarded(target)) {
      target = fixFollowedFieldofObjectwithInitialValue(fieldExpr,
                                                        objectExpr, target);
    }

This script collapses the block down to the single helper assignment above.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Iterable, List, Tuple

COMMENT_RE = re.compile(r"^(\s*)/\* begin followField:ofObject: \*/\s*$")
FETCH_RE = re.compile(
    r"^(\s*)([A-Za-z_][A-Za-z0-9_]*)\s*=\s*fetchPointerofObject\((.*)\);\s*$"
)
IF_RE = re.compile(
    r"^\s*if\s*\(\s*isOopForwarded\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)\s*\)\s*{\s*$"
)
FIX_FUNC = "fixFollowedFieldofObjectwithInitialValue"


def split_args(arg_string: str) -> Tuple[str, str]:
    depth = 0
    for idx, ch in enumerate(arg_string):
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        elif ch == "," and depth == 0:
            left = arg_string[:idx].strip()
            right = arg_string[idx + 1 :].strip()
            if not left or not right:
                break
            return left, right
    raise ValueError(f"Unable to split arguments: {arg_string!r}")


def split_three_args(arg_string: str) -> Tuple[str, str, str]:
    args: List[str] = []
    depth = 0
    start = 0
    for idx, ch in enumerate(arg_string):
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        elif ch == "," and depth == 0:
            args.append(arg_string[start:idx].strip())
            start = idx + 1
    args.append(arg_string[start:].strip())
    if len(args) != 3 or not all(args):
        raise ValueError(f"Unable to parse three arguments from {arg_string!r}")
    return args[0], args[1], args[2]


def join_block_lines(lines: List[str]) -> str:
    return "".join(lines).strip()


def transform_lines(lines: List[str], path: Path) -> Tuple[List[str], bool]:
    new_lines: List[str] = []
    i = 0
    changed = False

    while i < len(lines):
        comment_match = COMMENT_RE.match(lines[i])
        if not comment_match:
            new_lines.append(lines[i])
            i += 1
            continue

        if i + 2 >= len(lines):
            raise RuntimeError(f"Unexpected EOF after followField block in {path}")

        fetch_index = i + 1
        while fetch_index < len(lines) and lines[fetch_index].strip() == "":
            fetch_index += 1

        if fetch_index >= len(lines):
            raise RuntimeError(
                f"Missing fetchPointerofObject line after comment in {path}:{i + 1}"
            )

        fetch_line = lines[fetch_index].rstrip("\n")
        fetch_match = FETCH_RE.match(fetch_line)
        if not fetch_match:
            raise RuntimeError(
                f"Unable to parse fetchPointerofObject assignment in {path}:{fetch_index + 1}"
            )

        indent, target_var, args_str = fetch_match.groups()
        field_expr, object_expr = split_args(args_str)

        if_index = fetch_index + 1
        while if_index < len(lines) and lines[if_index].strip() == "":
            if_index += 1
        if if_index >= len(lines):
            raise RuntimeError(
                f"Missing if (isOopForwarded(...)) after fetch in {path}:{fetch_index + 1}"
            )

        if_line = lines[if_index].rstrip("\n")
        if_match = IF_RE.match(if_line)
        if not if_match or if_match.group(1) != target_var:
            raise RuntimeError(
                f"Unexpected if line for followField block in {path}:{if_index + 1}"
            )

        block_lines: List[str] = []
        j = if_index + 1
        while j < len(lines):
            line = lines[j]
            if line.strip() == "}":
                break
            block_lines.append(line)
            j += 1
        else:
            raise RuntimeError(
                f"Missing closing brace for followField block in {path}:{if_index + 1}"
            )

        statement = join_block_lines(block_lines)
        fix_re = re.compile(
            rf"^{re.escape(target_var)}\s*=\s*{FIX_FUNC}\(\s*(?P<args>.+)\)\s*;\s*$",
            re.DOTALL,
        )
        fix_match = fix_re.match(statement)
        if not fix_match:
            raise RuntimeError(
                f"Unable to parse fixFollowedFieldofObjectwithInitialValue call in {path}:{if_index + 2}"
            )

        fix_args = fix_match.group("args")
        fix_field, fix_object, fix_target = split_three_args(fix_args)
        if fix_target != target_var:
            raise RuntimeError(
                f"Unexpected target variable in fixFollowedField call "
                f"in {path}:{if_index + 2}: {fix_target} != {target_var}"
            )
        if fix_field != field_expr or fix_object != object_expr:
            raise RuntimeError(
                f"Field/object mismatch in followField block at {path}:{fetch_index + 1}"
            )

        new_lines.append(f"{indent}{target_var} = followFieldofObject({field_expr}, {object_expr});\n")
        i = j + 1
        changed = True

    return new_lines, changed


def process_file(path: Path) -> bool:
    original_lines = path.read_text().splitlines(keepends=True)
    transformed_lines, changed = transform_lines(original_lines, path)
    if changed:
        path.write_text("".join(transformed_lines))
        print(f"Updated {path}", file=sys.stderr)
    return changed


def iter_sources(paths: Iterable[Path]) -> Iterable[Path]:
    for path in paths:
        if path.is_dir():
            yield from path.rglob("*.c")
        elif path.suffix == ".c":
            yield path


def main(argv: List[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Replace inline followField:ofObject: sequences with followFieldofObject() calls."
    )
    parser.add_argument(
        "paths", nargs="+", help="Source files or directories to rewrite."
    )
    args = parser.parse_args(argv)

    any_changed = False
    for source in iter_sources(Path(p) for p in args.paths):
        changed = process_file(source)
        any_changed = any_changed or changed

    return 0 if any_changed else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
