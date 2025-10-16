#!/usr/bin/env python3
"""
Simplify `if` statements with constant conditions by removing branches that
will never execute.

For conditions that evaluate to true at compile time, the surrounding `if`
statement is replaced with its `then` body. For conditions that evaluate to
false, the (optional) `else` body replaces the entire construct; if no `else`
is present the statement is removed entirely.

The script relies on libclang via clang.cindex to evaluate the condition
expressions. It accepts a list of paths (files or directories) and rewrites all
`.c` sources it finds under them.
"""

from __future__ import annotations

import argparse
import sys
from ctypes import c_int, c_longlong, c_void_p
from pathlib import Path
from typing import Iterable, List, NamedTuple, Optional, Sequence, Tuple

from clang.cindex import (
    Config,
    Cursor,
    CursorKind,
    Index,
    TokenKind,
    TranslationUnit,
    conf,
)

# Location of libclang provided by Apple's Command Line Tools.
LIBCLANG_PATH = "/Library/Developer/CommandLineTools/usr/lib/libclang.dylib"

# Default compiler flags used when parsing the Spur Stack sources.
DEFAULT_CLANG_ARGS: Tuple[str, ...] = (
    "-std=gnu11",
    "-Iinclude",
    "-Isrc",
    "-Isrc/spur64.stack",
    "-Iplatforms/Cross/vm",
    "-Iplatforms/Cross/plugins/FilePlugin",
    "-Iplatforms/Cross/plugins/HostWindowPlugin",
    "-Iplatforms/Cross/plugins/SoundPlugin",
    "-Iplatforms/macOS/plugins/FilePlugin",
    "-Iplatforms/macOS/plugins/HostWindowPlugin",
    "-Iplatforms/macOS/plugins/SoundPlugin",
    "-Iplatforms/macOS/vm/Common",
    "-Iplatforms/macOS/vm/Common/Classes",
    "-Iplatforms/macOS/vm/OSX",
    "-Iplatforms/unix/vm",
    "-DDEBUGVM=0",
    "-DNDEBUG=1",
    "-DUSE_GLOBAL_STRUCT=0",
    "-DUSE_GLOBAL_STRUCT_REG=0",
    "-DSQUEAK_BUILTIN_PLUGIN=1",
    "-DBUILD_FOR_OSX=1",
    "-DUSE_METAL=1",
    "-DNO_ISNAN=1",
    "-DUSE_INLINE_MEMORY_ACCESSORS=1",
    "-include",
    "src/spur64.stack/interp.h",
)

# clang's CXEvalResultKind integer constants for results we care about.
CXEval_Int = 1


class Replacement(NamedTuple):
    start: int
    end: int
    text: str


def configure_libclang() -> None:
    """Initialise libclang and register the evaluation helpers we need."""

    Config.set_library_file(LIBCLANG_PATH)

    lib = conf.lib

    lib.clang_Cursor_Evaluate.restype = c_void_p
    lib.clang_Cursor_Evaluate.argtypes = [Cursor]
    lib.clang_EvalResult_getKind.restype = c_int
    lib.clang_EvalResult_getKind.argtypes = [c_void_p]
    lib.clang_EvalResult_getAsLongLong.restype = c_longlong
    lib.clang_EvalResult_getAsLongLong.argtypes = [c_void_p]
    lib.clang_EvalResult_dispose.argtypes = [c_void_p]


def evaluate_condition(cond: Cursor) -> Optional[bool]:
    """Evaluate a condition cursor; return True/False if it is constant."""

    lib = conf.lib
    handle = lib.clang_Cursor_Evaluate(cond)
    if not handle:
        return None
    try:
        kind = lib.clang_EvalResult_getKind(handle)
        if kind != CXEval_Int:
            return None
        value = lib.clang_EvalResult_getAsLongLong(handle)
        return bool(value)
    finally:
        lib.clang_EvalResult_dispose(handle)


def is_numeric_expression(source: str) -> bool:
    """Heuristically decide whether an expression contains only numeric tokens.

    We allow digits, hexadecimal prefixes, numeric suffixes, whitespace, and
    common operators. Any other character (e.g., alphabetic identifiers) causes
    the expression to be treated as non-constant to avoid folding macros or
    platform-dependent constructs (like sizeof checks).
    """

    allowed = set("0123456789abcdefABCDEFxXuUlL"
                  " \t\r\n()+-*/%<>=!&|^~?.:")
    return all(ch in allowed for ch in source)


def find_else_token_offset(
    tokens: Sequence, else_cursor: Cursor
) -> Optional[int]:
    """Return the byte offset of the `else` token for this branch, if any."""

    target = else_cursor.extent.start.offset
    else_offset: Optional[int] = None
    for token in tokens:
        off = token.extent.start.offset
        if off >= target:
            break
        if token.spelling == "else":
            else_offset = off
    return else_offset


def extract_then_text(
    source: str,
    cursor: Cursor,
    then_cursor: Cursor,
    else_cursor: Optional[Cursor],
    tokens: Sequence,
) -> str:
    """Extract the text for the `then` branch we want to keep."""

    body_start = then_cursor.extent.start.offset
    if else_cursor is not None:
        else_token_offset = find_else_token_offset(tokens, else_cursor)
        body_end = else_token_offset if else_token_offset is not None else else_cursor.extent.start.offset
    else:
        body_end = cursor.extent.end.offset

    snippet = source[body_start:body_end]
    snippet = snippet.rstrip(" \t")
    return snippet


def extract_else_text(
    source: str, cursor: Cursor, else_cursor: Cursor
) -> str:
    """Extract the text for the `else` branch to keep when the condition is false."""

    body_start = else_cursor.extent.start.offset
    body_end = cursor.extent.end.offset
    return source[body_start:body_end]


def gather_replacements_for_file(
    tu: TranslationUnit, path: Path, source: str
) -> List[Replacement]:
    """Inspect all IfStmt nodes in `path` and collect constant-condition rewrites."""

    replacements: List[Replacement] = []
    path_resolved = path.resolve()

    for cursor in tu.cursor.walk_preorder():
        if cursor.kind != CursorKind.IF_STMT:
            continue

        start_file = cursor.extent.start.file
        if start_file is None or Path(start_file.name).resolve() != path_resolved:
            continue

        children = list(cursor.get_children())
        if not children:
            continue

        cond_cursor: Optional[Cursor] = children[0] if len(children) >= 1 else None
        then_cursor: Optional[Cursor] = children[1] if len(children) >= 2 else None
        else_cursor: Optional[Cursor] = children[2] if len(children) >= 3 else None

        if cond_cursor is None or then_cursor is None:
            continue

        cond_tokens = list(cond_cursor.get_tokens())
        if any(tok.kind == TokenKind.IDENTIFIER for tok in cond_tokens):
            continue

        cond_text = source[cond_cursor.extent.start.offset : cond_cursor.extent.end.offset]
        if not is_numeric_expression(cond_text):
            continue

        constant_value = evaluate_condition(cond_cursor)
        if constant_value is None:
            continue

        tokens = list(cursor.get_tokens())
        if constant_value:
            replacement_text = extract_then_text(
                source, cursor, then_cursor, else_cursor, tokens
            )
        else:
            if else_cursor is None:
                replacement_text = ""
            else:
                replacement_text = extract_else_text(source, cursor, else_cursor)

        replacements.append(
            Replacement(
                cursor.extent.start.offset,
                cursor.extent.end.offset,
                replacement_text,
            )
        )

    return replacements


def apply_replacements(source: str, replacements: Sequence[Replacement]) -> str:
    """Apply the list of replacements (sorted from last to first)."""

    if not replacements:
        return source

    result = source
    for start, end, text in sorted(replacements, key=lambda item: item[0], reverse=True):
        result = result[:start] + text + result[end:]
    return result


def iter_source_files(paths: Iterable[Path]) -> Iterable[Path]:
    """Yield every `.c` file under the provided paths."""

    for path in paths:
        if path.is_dir():
            yield from sorted(path.rglob("*.c"))
        elif path.suffix == ".c":
            yield path


def process_file(path: Path, index: Index, clang_args: Sequence[str]) -> bool:
    """Parse, rewrite, and write back `path` if changes were made."""

    try:
        tu = index.parse(
            str(path),
            args=list(clang_args),
            options=TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD,
        )
    except Exception as exc:  # pragma: no cover - diagnostic path
        print(f"[skip] {path}: failed to parse ({exc})", file=sys.stderr)
        return False

    source = path.read_text()
    replacements = gather_replacements_for_file(tu, path, source)
    if not replacements:
        return False

    updated = apply_replacements(source, replacements)
    if updated != source:
        path.write_text(updated)
        return True
    return False


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Remove constant-condition if statements using libclang."
    )
    parser.add_argument(
        "paths",
        nargs="*",
        type=Path,
        default=[Path("src/spur64.stack")],
        help="Files or directories to process (default: src/spur64.stack).",
    )
    parser.add_argument(
        "--clang-arg",
        action="append",
        dest="clang_args",
        default=[],
        help="Additional clang argument to pass during parsing.",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    configure_libclang()
    index = Index.create()

    clang_args: Tuple[str, ...] = DEFAULT_CLANG_ARGS + tuple(args.clang_args)

    changed_files: List[Path] = []
    for file_path in iter_source_files(args.paths):
        if process_file(file_path, index, clang_args):
            changed_files.append(file_path)

    if changed_files:
        print("Updated:")
        for path in changed_files:
            print(f"  {path}")
    else:
        print("No constant-condition if statements found.")


if __name__ == "__main__":
    main()
