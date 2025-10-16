#!/usr/bin/env python3
"""
Replace hand-inlined followForwarded logic with calls to followForwarded().

The script looks for sequences of the form:

    /* begin followForwarded: */
    assert(isUnambiguouslyForwarder(var));
    temp = longAt((void *)((var + BaseHeaderSize) + (0U << (shiftForWord()))));
    while (isOopForwarded(temp)) {
      temp = longAt((void *)((temp + BaseHeaderSize) + (0U << (shiftForWord()))));
    }
    ...

and rewrites them to the appropriate followForwarded(var) invocation.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path
from typing import List

COMMENT_RE = re.compile(r"^(\s*)/\* begin followForwarded: \*/\s*$")
ASSERT_RE = re.compile(
    r"^\s*assert\(isUnambiguouslyForwarder\((?P<arg>[A-Za-z_][A-Za-z0-9_]*)\)\);\s*$"
)
ASSIGN_INLINE_RE = re.compile(r"^(\s*)([A-Za-z_][A-Za-z0-9_]*) = longAt\(")
ASSIGN_SPLIT_RE = re.compile(r"^(\s*)([A-Za-z_][A-Za-z0-9_]*) =\s*$")
WHILE_RE = re.compile(
    r"^\s*while \(isOopForwarded\(([A-Za-z_][A-Za-z0-9_]*)\)\) {\s*$"
)
INNER_ASSIGN_RE = re.compile(r"^\s*([A-Za-z_][A-Za-z0-9_]*) = longAt\(")
RETURN_RE = re.compile(r"^(\s*)return ([A-Za-z_][A-Za-z0-9_]*)\s*;\s*$")
SIMPLE_ASSIGN_RE = re.compile(
    r"^(\s*)([A-Za-z_][A-Za-z0-9_]*)\s*=\s*([A-Za-z_][A-Za-z0-9_]*)\s*;\s*$"
)


def transform_lines(lines: List[str], path: Path) -> List[str]:
    new_lines: List[str] = []
    i = 0
    replacements = 0

    while i < len(lines):
        comment_match = COMMENT_RE.match(lines[i])
        if not comment_match:
            new_lines.append(lines[i])
            i += 1
            continue

        if i + 4 >= len(lines):
            raise RuntimeError(f"Unexpected EOF after followForwarded block in {path}")

        assert_match = ASSERT_RE.match(lines[i + 1])
        if not assert_match:
            raise RuntimeError(
                f"Unable to parse assert line after followForwarded comment in {path}:{i + 2}"
            )
        arg_var = assert_match.group("arg")

        assign_line_index = i + 2
        first_longat_index = assign_line_index

        assign_match = ASSIGN_INLINE_RE.match(lines[assign_line_index])
        if assign_match:
            assign_indent, temp_var = assign_match.groups()
        else:
            split_match = ASSIGN_SPLIT_RE.match(lines[assign_line_index])
            if not split_match:
                raise RuntimeError(
                    f"Unable to parse initial followForwarded assignment in "
                    f"{path}:{assign_line_index + 1}"
                )
            assign_indent, temp_var = split_match.groups()
            first_longat_index += 1
            if first_longat_index >= len(lines):
                raise RuntimeError(
                    f"Unexpected EOF while parsing longAt line in {path}:{assign_line_index + 1}"
                )
            if "longAt" not in lines[first_longat_index]:
                raise RuntimeError(
                    f"Expected longAt after split assignment in "
                    f"{path}:{first_longat_index + 1}"
                )

        while_index = first_longat_index + 1
        while while_index < len(lines) and "while (isOopForwarded(" not in lines[while_index]:
            while_index += 1
        if while_index >= len(lines):
            raise RuntimeError(
                f"Unable to locate while loop for {temp_var} in {path}:{first_longat_index + 1}"
            )

        if not any(
            f"{arg_var} + BaseHeaderSize" in lines[idx]
            for idx in range(first_longat_index, while_index)
        ):
            raise RuntimeError(
                f"Initial longAt assignment does not reference {arg_var} as expected "
                f"in {path}:{first_longat_index + 1}"
            )

        while_match = WHILE_RE.match(lines[while_index])
        if not while_match or while_match.group(1) != temp_var:
            raise RuntimeError(
                f"Unable to parse while loop for {temp_var} in {path}:{while_index + 1}"
            )

        inner_index = while_index + 1
        while inner_index < len(lines) and "longAt" not in lines[inner_index]:
            if lines[inner_index].strip() == "}":
                break
            inner_index += 1
        if inner_index >= len(lines) or lines[inner_index].strip() == "}":
            raise RuntimeError(
                f"Unable to find inner longAt assignment for {temp_var} after while in "
                f"{path}:{while_index + 1}"
            )

        inner_match = INNER_ASSIGN_RE.match(lines[inner_index])
        if not inner_match or inner_match.group(1) != temp_var:
            raise RuntimeError(
                f"Unable to parse inner longAt assignment for {temp_var} in "
                f"{path}:{inner_index + 1}"
            )

        # Locate the end of the while block.
        j = inner_index + 1
        while j < len(lines) and lines[j].strip() != "}":
            j += 1
        if j >= len(lines):
            raise RuntimeError(
                f"While block for followForwarded starting at {path}:{i + 1} "
                "is not closed"
            )

        block_end = j

        # Collect blank lines between the block and the following statement.
        blank_lines: List[str] = []
        k = block_end + 1
        while k < len(lines) and lines[k].strip() == "":
            blank_lines.append(lines[k])
            k += 1

        next_line_index = k if k < len(lines) else None

        # Determine replacement.
        replacement_line = None

        if next_line_index is not None:
            next_line = lines[next_line_index]
            return_match = RETURN_RE.match(next_line)
            assign_after_match = SIMPLE_ASSIGN_RE.match(next_line)

            if assign_after_match and assign_after_match.group(3) == temp_var:
                assign_after_indent, final_var, _ = assign_after_match.groups()
                replacement_line = (
                    f"{assign_after_indent}{final_var} = "
                    f"followForwarded({arg_var});\n"
                )
                new_lines.append(replacement_line)
                for blank in blank_lines:
                    new_lines.append(blank)
                i = next_line_index + 1
            elif return_match and return_match.group(2) == temp_var:
                for blank in blank_lines:
                    new_lines.append(blank)
                ret_indent = return_match.group(1)
                new_lines.append(
                    f"{ret_indent}return followForwarded({arg_var});\n"
                )
                i = next_line_index + 1
            else:
                new_lines.append(
                    f"{assign_indent}{temp_var} = followForwarded({arg_var});\n"
                )
                for blank in blank_lines:
                    new_lines.append(blank)
                i = block_end + 1 if next_line_index is None else next_line_index
        else:
            new_lines.append(
                f"{assign_indent}{temp_var} = followForwarded({arg_var});\n"
            )
            i = block_end + 1

        replacements += 1

    if replacements == 0:
        return lines

    return new_lines


def process_file(path: Path) -> bool:
    original = path.read_text().splitlines(keepends=True)
    transformed = transform_lines(original, path)
    if transformed != original:
        path.write_text("".join(transformed))
        print(f"Updated {path}", file=sys.stderr)
        return True
    return False


def main(argv: List[str]) -> int:
    parser = argparse.ArgumentParser(
        description="Un-inline followForwarded blocks in C sources."
    )
    parser.add_argument("files", nargs="+", help="Source files to process.")
    args = parser.parse_args(argv)

    changed = False
    for name in args.files:
        path = Path(name)
        if not path.is_file():
            continue
        if process_file(path):
            changed = True

    return 0 if changed else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
