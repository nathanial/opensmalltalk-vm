#!/bin/bash
# Script to update include paths in interp.c

cd "$(dirname "$0")"

echo "Generating file location map..."

# Create a temporary file to store the mapping
MAPFILE=$(mktemp)

# Find all .c files in interp subdirectories and create a mapping
find interp -name "*.c" -type f | while read filepath; do
    filename=$(basename "$filepath")
    # Store: filename -> new_path
    echo "$filename|$filepath" >> "$MAPFILE"
done

echo "Found $(wc -l < "$MAPFILE") files to remap"
echo "Updating interp.c includes..."

# Create a backup
cp interp.c interp.c.bak

# Process each line in the map file
while IFS='|' read -r filename newpath; do
    # Escape forward slashes for sed
    escaped_newpath=$(echo "$newpath" | sed 's/\//\\\//g')
    # Replace old path with new path
    sed -i '' "s/#include \"interp\/$filename\"/#include \"$escaped_newpath\"/g" interp.c
done < "$MAPFILE"

# Clean up
rm "$MAPFILE"

echo "Include paths updated!"
echo "Original file backed up as interp.c.bak"
echo ""
echo "Checking for any remaining unmapped includes..."
REMAINING=$(grep -c '#include "interp/[^/]*\.c"' interp.c || echo "0")
if [ "$REMAINING" -gt "0" ]; then
    echo "WARNING: $REMAINING includes were not updated (may be in root interp/)"
    grep '#include "interp/[^/]*\.c"' interp.c | head -10
else
    echo "All includes successfully updated!"
fi
