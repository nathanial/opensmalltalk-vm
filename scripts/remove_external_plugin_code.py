#!/usr/bin/env python3
"""
Remove code paths for external plugins (when SQUEAK_BUILTIN_PLUGIN is not defined).
Since all plugins are now internal, SQUEAK_BUILTIN_PLUGIN is always defined.

This script:
1. Removes #else branches after #ifdef SQUEAK_BUILTIN_PLUGIN
2. Removes #if !defined(SQUEAK_BUILTIN_PLUGIN) blocks
3. Keeps #ifdef SQUEAK_BUILTIN_PLUGIN blocks and removes the directives
4. Simplifies the code by removing now-unnecessary conditionals
"""

import re
import sys
from pathlib import Path

def remove_external_plugin_code(content):
    """Remove external plugin code paths from C source."""
    lines = content.split('\n')
    result = []
    i = 0

    # Track ifdef/endif nesting
    skip_depth = 0
    keep_depth = 0

    while i < len(lines):
        line = lines[i]
        stripped = line.strip()

        # Handle #ifdef SQUEAK_BUILTIN_PLUGIN
        if re.match(r'#\s*ifdef\s+SQUEAK_BUILTIN_PLUGIN', stripped):
            # Keep content, skip the directive
            keep_depth += 1
            i += 1
            continue

        # Handle #if !defined(SQUEAK_BUILTIN_PLUGIN)
        if re.match(r'#\s*if\s+!defined\s*\(\s*SQUEAK_BUILTIN_PLUGIN\s*\)', stripped):
            # Skip this entire block
            skip_depth = 1
            i += 1
            while i < len(lines) and skip_depth > 0:
                line = lines[i]
                stripped = line.strip()
                if re.match(r'#\s*if', stripped):
                    skip_depth += 1
                elif re.match(r'#\s*endif', stripped):
                    skip_depth -= 1
                    if skip_depth == 0:
                        i += 1
                        break
                elif skip_depth == 1 and re.match(r'#\s*else', stripped):
                    # Found #else at our level - keep the rest until #endif
                    i += 1
                    while i < len(lines):
                        line = lines[i]
                        stripped = line.strip()
                        if re.match(r'#\s*if', stripped):
                            skip_depth += 1
                        elif re.match(r'#\s*endif', stripped):
                            skip_depth -= 1
                            if skip_depth == 0:
                                i += 1
                                break
                        result.append(lines[i])
                        i += 1
                    break
                i += 1
            continue

        # Handle #else when we're in a keep block
        if keep_depth > 0 and re.match(r'#\s*else', stripped):
            # Skip from #else to #endif
            skip_depth = 1
            i += 1
            while i < len(lines) and skip_depth > 0:
                line = lines[i]
                stripped = line.strip()
                if re.match(r'#\s*if', stripped):
                    skip_depth += 1
                elif re.match(r'#\s*endif', stripped):
                    skip_depth -= 1
                    if skip_depth == 0:
                        keep_depth -= 1
                        i += 1
                        break
                i += 1
            continue

        # Handle #endif when we're in a keep block
        if keep_depth > 0 and re.match(r'#\s*endif', stripped):
            keep_depth -= 1
            i += 1
            continue

        # Normal line - keep it
        result.append(line)
        i += 1

    return '\n'.join(result)


def process_file(filepath):
    """Process a single file."""
    print(f"Processing {filepath}")

    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()

    # Check if file contains SQUEAK_BUILTIN_PLUGIN
    if 'SQUEAK_BUILTIN_PLUGIN' not in content:
        print(f"  Skipping {filepath} - no SQUEAK_BUILTIN_PLUGIN references")
        return

    original_content = content
    content = remove_external_plugin_code(content)

    if content != original_content:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"  Modified {filepath}")
    else:
        print(f"  No changes needed for {filepath}")


def main():
    if len(sys.argv) > 1:
        # Process specific files
        for filepath in sys.argv[1:]:
            process_file(Path(filepath))
    else:
        print("Usage: remove_external_plugin_code.py <file1> [file2 ...]")
        sys.exit(1)


if __name__ == '__main__':
    main()
