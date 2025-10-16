#!/usr/bin/env python3
"""
Remove SPURVM preprocessor conditionals, keeping only SPURVM code paths.
"""

import sys
import re

def process_file(filepath):
    with open(filepath, 'r') as f:
        lines = f.readlines()

    result = []
    i = 0
    in_spurvm_block = False
    in_else_block = False
    skip_until_endif = False

    while i < len(lines):
        line = lines[i]

        # Check for #if SPURVM
        if re.match(r'^#if SPURVM\s*$', line):
            in_spurvm_block = True
            i += 1
            continue

        # Check for #else  /* SPURVM */
        if in_spurvm_block and re.match(r'^#else\s+/\*\s*SPURVM\s*\*/\s*$', line):
            in_else_block = True
            i += 1
            continue

        # Check for #endif /* SPURVM */
        if (in_spurvm_block or in_else_block) and re.match(r'^#endif\s+/\*\s*SPURVM\s*\*/\s*$', line):
            in_spurvm_block = False
            in_else_block = False
            i += 1
            continue

        # If we're in the else block, skip the line (non-SPURVM code)
        if in_else_block:
            i += 1
            continue

        # Otherwise, keep the line
        result.append(line)
        i += 1

    return result

def remove_fallback_macros(lines):
    """Remove the fallback macro definitions that are no longer needed."""
    result = []
    i = 0
    skip_block = False

    while i < len(lines):
        line = lines[i]

        # Check for the start of the fallback macro block
        if re.match(r'^#if !defined\(isKindOfInteger\)\s*$', line):
            skip_block = True
            i += 1
            continue

        # Check for the end of the fallback macro block
        if skip_block and re.match(r'^#endif\s*$', line):
            # Peek ahead to see if this is the right endif
            # The fallback block ends when we see an endif after the macro definitions
            skip_block = False
            i += 1
            continue

        # Skip lines in the fallback block
        if skip_block:
            i += 1
            continue

        result.append(line)
        i += 1

    return result

def main():
    if len(sys.argv) != 2:
        print("Usage: remove_spurvm_conditionals.py <file>")
        sys.exit(1)

    filepath = sys.argv[1]

    # First pass: remove SPURVM conditionals
    lines = process_file(filepath)

    # Second pass: remove fallback macros
    lines = remove_fallback_macros(lines)

    # Write the result
    with open(filepath, 'w') as f:
        f.writelines(lines)

    print(f"Processed {filepath}")

if __name__ == '__main__':
    main()
