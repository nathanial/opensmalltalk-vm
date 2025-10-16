#!/usr/bin/env python3
"""
Rewrite simple goto-based fall-through chains into structured conditionals.

Pattern handled:

    if (condA) {
        ...;
        goto target;
    }
    if (condB) {
        ...;
        goto target;
    }
    ... // optional default statements, no labels
target:

becomes

    if (condA) {
        ...;
    } else if (condB) {
        ...;
    } else {
        ...; // default block if present
    }
target:

The script is conservative: it only transforms sequences that live within a
single compound statement, where each `if`'s then-branch is a compound whose
final statement is `goto target`, and the default block (the statements
between the final `if` and the label) contains no labels.

After applying replacements, run clang-format to tidy indentation.
"""

from __future__ import annotations

import argparse
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, Iterator, List, Optional, Sequence, Tuple

from clang.cindex import (
    Config,
    Cursor,
    CursorKind,
    Index,
    Token,
    TranslationUnit,
    TranslationUnitLoadError,
)

# Location of libclang from Apple's Command Line Tools.
LIBCLANG_PATH = "/Library/Developer/CommandLineTools/usr/lib/libclang.dylib"

# Compiler arguments mirroring the Spur stack VM build.
DEFAULT_CLANG_ARGS: Tuple[str, ...] = (
    "-std=gnu11",
    "-Iinclude",
    "-Isrc",
    "-Isrc/spur64.stack",
    "-Iplatform/vm",
    "-Iplatform/plugins/FilePlugin",
    "-Iplatform/plugins/HostWindowPlugin",
    "-Iplatform/plugins/SoundPlugin",
    "-Iplatform/OSX",
    "-Iplatform/Common",
    "-Iplatform/Common/Classes",
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


@dataclass
class Branch:
    header: str  # text of `if (...)`
    body: str  # inner statements (without braces), goto removed
    start: int
    end: int
    indent_step: str


@dataclass
class Replacement:
    start: int
    end: int
    text: str


def configure_libclang() -> None:
    Config.set_library_file(LIBCLANG_PATH)


def iter_c_files(paths: Sequence[Path]) -> Iterator[Path]:
    for path in paths:
        if path.is_dir():
            yield from iter_c_files(sorted(path.iterdir()))
        elif path.suffix == ".c":
            yield path


def same_file(cursor: Cursor, target_path: Path) -> bool:
    loc = cursor.location
    return loc.file is not None and Path(loc.file.name).resolve() == target_path


def tokens_between(node: Cursor) -> List[Token]:
    return list(node.get_tokens())


def detect_indent_step(body_text: str, indent: str) -> str:
    for line in body_text.splitlines():
        stripped = line.strip()
        if not stripped:
            continue
        if line.startswith(indent):
            remainder = line[len(indent) :]
        else:
            remainder = line
        leading = remainder[: len(remainder) - len(remainder.lstrip("\t "))]
        if leading:
            return leading
    return "  "


def extract_branch(
    if_cursor: Cursor, label: str, source: str
) -> Optional[Branch]:
    # Only handle `if` with compound then-branch and no else.
    children = list(if_cursor.get_children())
    if len(children) < 2:
        return None
    condition = children[0]
    then_branch = children[1]
    else_branch = children[2] if len(children) >= 3 else None
    if else_branch is not None:
        return None
    if then_branch.kind != CursorKind.COMPOUND_STMT:
        return None

    statements = [c for c in then_branch.get_children()]
    if not statements or statements[-1].kind != CursorKind.GOTO_STMT:
        return None

    goto_cursor = statements[-1]
    # Verify the goto jumps to the expected label.
    goto_tokens = tokens_between(goto_cursor)
    target = None
    for i, tok in enumerate(goto_tokens):
        if tok.spelling == "goto" and i + 1 < len(goto_tokens):
            target = goto_tokens[i + 1].spelling
            break
    if target != label:
        return None

    if_start = if_cursor.extent.start.offset
    body_tokens = tokens_between(then_branch)
    if not body_tokens or body_tokens[0].spelling != "{" or body_tokens[-1].spelling != "}":
        return None

    line_start = source.rfind("\n", 0, if_start) + 1
    header_indent = source[line_start:if_start]
    cond_text = source[if_start : then_branch.extent.start.offset].strip()

    body_start = body_tokens[0].extent.end.offset
    body_end = body_tokens[-1].extent.start.offset
    body_text = source[body_start:body_end]

    goto_start = goto_cursor.extent.start.offset
    goto_end = goto_cursor.extent.end.offset
    rel_start = goto_start - body_start
    rel_end = goto_end - body_start
    if not (0 <= rel_start <= rel_end <= len(body_text)):
        return None

    # Expand to the beginning of the line containing the goto.
    line_start = body_text.rfind("\n", 0, rel_start) + 1
    trimmed_start = line_start
    trimmed_end = rel_end
    if trimmed_end < len(body_text) and body_text[trimmed_end : trimmed_end + 1] == ";":
        trimmed_end += 1
    while trimmed_end < len(body_text) and body_text[trimmed_end] in " \t":
        trimmed_end += 1
    if trimmed_end < len(body_text) and body_text[trimmed_end] == "\n":
        trimmed_end += 1

    body_without_goto = body_text[:trimmed_start] + body_text[trimmed_end:]
    body_without_goto = body_without_goto.strip("\n")
    indent_step = detect_indent_step(body_text, header_indent)

    return Branch(
        header=cond_text,
        body=body_without_goto,
        start=if_cursor.extent.start.offset,
        end=then_branch.extent.end.offset,
        indent_step=indent_step,
    )


def reindent_block(block: str, indent: str, indent_step: str) -> str:
    if not block:
        return ""
    inner_indent = indent + indent_step
    lines = block.splitlines()
    result: List[str] = []
    for line in lines:
        stripped = line.strip()
        if not stripped:
            result.append("")
            continue
        trimmed = line
        for prefix in (inner_indent, indent):
            if trimmed.startswith(prefix):
                trimmed = trimmed[len(prefix) :]
                break
        else:
            trimmed = trimmed.lstrip("\t ")
        result.append(inner_indent + trimmed)
    return "\n".join(result)


def build_conditional(branches: List[Branch], default_text: str, indent: str) -> str:
    pieces: List[str] = []
    for idx, branch in enumerate(branches):
        prefix = indent + ("else " if idx > 0 else "")
        block_lines = [f"{prefix}{branch.header} {{"]
        body = reindent_block(branch.body, indent, branch.indent_step)
        if body:
            block_lines.append(body)
        block_lines.append(f"{indent}}}")
        pieces.append("\n".join(block_lines))

    default_body = default_text.strip("\n")
    if default_body:
        default_block = reindent_block(
            default_body, indent, branches[0].indent_step
        )
        pieces.append("\n".join([f"{indent}else {{", default_block, f"{indent}}}"]))
    result = "\n".join(pieces)
    if not result.endswith("\n"):
        result += "\n"
    return result


def block_contains_label(statements: List[Cursor]) -> bool:
    return any(stmt.kind == CursorKind.LABEL_STMT for stmt in statements)


def process_compound(
    compound: Cursor, target_path: Path, source: str, seen_labels: set[int]
) -> List[Replacement]:
    statements = [
        stmt
        for stmt in compound.get_children()
        if same_file(stmt, target_path)
        and stmt.extent.start.offset < stmt.extent.end.offset
    ]
    statements.sort(key=lambda c: c.extent.start.offset)

    replacements: List[Replacement] = []

    for idx, stmt in enumerate(statements):
        if stmt.kind != CursorKind.LABEL_STMT:
            continue
        label_name = stmt.spelling or stmt.displayname
        if not label_name:
            continue
        label_offset = stmt.extent.start.offset
        if label_offset in seen_labels:
            continue

        # Collect default block statements (immediately preceding and not IF).
        default_indices: List[int] = []
        cursor_idx = idx - 1
        while cursor_idx >= 0:
            prev_stmt = statements[cursor_idx]
            if prev_stmt.kind == CursorKind.LABEL_STMT:
                break
            if prev_stmt.kind != CursorKind.IF_STMT:
                default_indices.append(cursor_idx)
                cursor_idx -= 1
                continue
            break
        default_indices.reverse()

        # Collect candidate IF statements.
        branch_indices: List[int] = []
        while cursor_idx >= 0 and statements[cursor_idx].kind == CursorKind.IF_STMT:
            if_branch = statements[cursor_idx]
            branch = extract_branch(if_branch, label_name, source)
            if branch is None:
                break
            branch_indices.append(cursor_idx)
            cursor_idx -= 1
        branch_indices.reverse()

        if not branch_indices:
            continue

        # Do not rewrite if default block contains a label (control-flow tricky).
        if default_indices:
            default_stmts = [statements[i] for i in default_indices]
            if block_contains_label(default_stmts):
                continue
            default_start = default_stmts[0].extent.start.offset
            default_end = default_stmts[-1].extent.end.offset
        else:
            default_start = stmt.extent.start.offset
            default_end = stmt.extent.start.offset

        # Extract branch details.
        branches: List[Branch] = []
        valid = True
        for i in branch_indices:
            info = extract_branch(statements[i], label_name, source)
            if info is None:
                valid = False
                break
            branches.append(info)
        if not valid:
            continue

        start_offset = branches[0].start
        # Compose default text from source between end of last branch and label.
        last_branch_end = branches[-1].end
        default_text = source[last_branch_end:stmt.extent.start.offset]

        indent_line_start = source.rfind("\n", 0, start_offset) + 1
        indent = source[indent_line_start:start_offset]

        replacement_text = build_conditional(branches, default_text, indent)
        replacements.append(Replacement(start_offset, default_end, replacement_text))
        seen_labels.add(label_offset)

    return replacements


def apply_replacements(source: str, replacements: Sequence[Replacement]) -> str:
    updated = source
    for repl in sorted(replacements, key=lambda r: r.start, reverse=True):
        updated = updated[: repl.start] + repl.text + updated[repl.end :]
    return updated


def process_file(index: Index, path: Path) -> bool:
    try:
        tu = index.parse(
            str(path),
            args=DEFAULT_CLANG_ARGS,
            options=TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD,
        )
    except TranslationUnitLoadError as exc:
        print(f"[warn] failed to parse {path}: {exc}", file=sys.stderr)
        return False

    source = path.read_text()
    replacements: List[Replacement] = []
    seen_labels: set[int] = set()
    target_path = path.resolve()

    def visit(node: Cursor) -> None:
        in_file = same_file(node, target_path)
        if in_file and node.kind == CursorKind.COMPOUND_STMT:
            replacements.extend(process_compound(node, target_path, source, seen_labels))
        for child in node.get_children():
            visit(child)

    visit(tu.cursor)

    if not replacements:
        return False

    updated = apply_replacements(source, replacements)
    if updated != source:
        path.write_text(updated)
        return True
    return False


def run_clang_format(paths: Sequence[Path]) -> None:
    if not paths:
        return
    cmd = ["./format.sh", *(str(p) for p in paths)]
    subprocess.run(cmd, check=True)


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Refactor simple goto-based fall-through chains."
    )
    parser.add_argument(
        "paths",
        nargs="*",
        default=[Path("src")],
        type=Path,
        help="Files or directories to process (default: src)",
    )
    parser.add_argument(
        "--no-format",
        action="store_true",
        help="Do not run ./format.sh after rewriting",
    )
    args = parser.parse_args()

    configure_libclang()
    index = Index.create()

    c_files = list(iter_c_files([p.resolve() for p in args.paths]))
    touched: List[Path] = []
    for file in c_files:
        if process_file(index, file):
            touched.append(file)

    print(f"Updated {len(touched)} files.")

    if touched and not args.no_format:
        run_clang_format(touched)


if __name__ == "__main__":
    main()
