#!/usr/bin/env python3
"""
Replace hand-inlined followObjField:ofObject: sequences with calls to
followObjFieldofObject(fieldIndex, object).

Many generated Spur interpreter sources expand the helper as:

    /* begin followObjField:ofObject: */
    target = fetchPointerofObject(fieldExpr, objectExpr);
    assert(isNonImmediate(target));
    if ((!((longAt((void *)(target))) &
           ((classIndexMask()) - (isForwardedObjectClassIndexPun()))))) {
      target = fixFollowedFieldofObjectwithInitialValue(fieldExpr,
                                                        objectExpr, target);
    }

This script collapses the block down to a single helper assignment.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import Iterable, List, Tuple

COMMENT_RE = re.compile(r"^(\s*)/\* begin followObjField:ofObject: \*/\s*$")
FETCH_RE = re.compile(
    r"^([A-Za-z_][A-Za-z0-9_]*)\s*=\s*fetchPointerofObject\((.*)\);\s*$"
)
ASSERT_RE = re.compile(
    r"^\s*assert\s*\(\s*isNonImmediate\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)\s*\)\s*;\s*$"
)
IF_PREFIX_RE = re.compile(r"^\s*if\s*\(")
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


def collect_braced_block(lines: List[str], start_index: int) -> Tuple[str, int]:
    """
    Return the block contents (without surrounding braces) and the index just
    after the closing brace. Handles braces on the if-line or following lines.
    """
    block_lines: List[str] = []
    i = start_index
    while i < len(lines):
        line = lines[i]
        brace_pos = line.find("{")
        if brace_pos != -1:
            block_lines.append(line[brace_pos + 1 :])
            i += 1
            break
        i += 1
    else:
        raise RuntimeError("Expected '{' to start followObjField block")

    depth = 1
    while i < len(lines) and depth > 0:
        line = lines[i]
        open_count = line.count("{")
        close_count = line.count("}")
        depth += open_count
        depth -= close_count
        if depth < 0:
            raise RuntimeError("Unbalanced braces in followObjField block")
        if depth == 0:
            brace_end = line.rfind("}")
            if brace_end == -1:
                raise RuntimeError("Expected closing brace in followObjField block")
            block_lines.append(line[:brace_end])
            i += 1
            break
        block_lines.append(line)
        i += 1

    if depth != 0:
        raise RuntimeError("Unterminated followObjField block")

    return "".join(block_lines), i


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

        fetch_index = i + 1
        while fetch_index < len(lines) and lines[fetch_index].strip() == "":
            fetch_index += 1
        if fetch_index >= len(lines):
            raise RuntimeError(
                f"Missing fetchPointerofObject line after comment in {path}:{i + 1}"
            )

        fetch_lines: List[str] = []
        j = fetch_index
        while j < len(lines):
            fetch_lines.append(lines[j].rstrip("\n"))
            if ";" in lines[j]:
                break
            j += 1
        else:
            raise RuntimeError(
                f"Missing semicolon terminating fetchPointerofObject call in {path}:{fetch_index + 1}"
            )

        fetch_text = " ".join(" ".join(fetch_lines).split())
        fetch_match = FETCH_RE.match(fetch_text)
        if not fetch_match:
            raise RuntimeError(
                f"Unable to parse fetchPointerofObject assignment in {path}:{fetch_index + 1}"
            )

        target_var, args_str = fetch_match.groups()
        field_expr, object_expr = split_args(args_str)
        assignment_indent_match = re.match(r"^(\s*)", lines[fetch_index])
        assignment_indent = assignment_indent_match.group(1) if assignment_indent_match else ""

        assert_index = j + 1
        while assert_index < len(lines) and lines[assert_index].strip() == "":
            assert_index += 1
        if assert_index >= len(lines):
            raise RuntimeError(
                f"Missing assert line after fetch in {path}:{fetch_index + 1}"
            )

        assert_line = lines[assert_index].rstrip("\n")
        assert_match = ASSERT_RE.match(assert_line)
        if not assert_match or assert_match.group(1) != target_var:
            raise RuntimeError(
                f"Unexpected assert line for followObjField block in {path}:{assert_index + 1}"
            )

        if_index = assert_index + 1
        while if_index < len(lines) and lines[if_index].strip() == "":
            if_index += 1
        if if_index >= len(lines):
            raise RuntimeError(
                f"Missing if clause after assert in {path}:{assert_index + 1}"
            )

        if_line = lines[if_index].rstrip("\n")
        if not IF_PREFIX_RE.match(if_line):
            raise RuntimeError(
                f"Expected if clause for followObjField block in {path}:{if_index + 1}"
            )
        if target_var not in if_line:
            raise RuntimeError(
                f"Target variable {target_var} missing from if condition in {path}:{if_index + 1}"
            )

        # Collect the block following the if statement.
        block_content, next_index = collect_braced_block(lines, if_index)
        normalized_block = " ".join(block_content.split())
        fix_re = re.compile(
            rf"^{re.escape(target_var)}\s*=\s*{FIX_FUNC}\(\s*(?P<args>.+)\s*\)\s*;\s*$"
        )
        fix_match = fix_re.match(normalized_block)
        if not fix_match:
            raise RuntimeError(
                f"Unable to parse fixFollowedField call in {path}:{if_index + 1}"
            )
        fix_args = fix_match.group("args")
        fix_field, fix_object, fix_target = split_three_args(fix_args)
        if fix_target != target_var:
            raise RuntimeError(
                f"Unexpected target in fixFollowedField call in {path}:{if_index + 1}"
            )
        if fix_field != field_expr or fix_object != object_expr:
            raise RuntimeError(
                f"Mismatched field/object expressions in followObjField block at {path}:{fetch_index + 1}"
            )

        new_lines.append(
            f"{assignment_indent}{target_var} = followObjFieldofObject({field_expr}, {object_expr});\n"
        )
        i = next_index
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
        description="Replace inline followObjField:ofObject: sequences with followObjFieldofObject() calls."
    )
    parser.add_argument(
        "paths", nargs="+", help="Source files or directories to rewrite."
    )
    args = parser.parse_args(argv)

    any_changed = False
    for source in iter_sources(Path(p) for p in args.paths):
        if "followObjFieldofObject.c" in source.name:
            continue
        changed = process_file(source)
        any_changed = any_changed or changed

    return 0 if any_changed else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
