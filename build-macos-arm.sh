#!/bin/bash
set -e

# Build script for OpenSmalltalk VM on macOS ARM64
# This script builds the Squeak Cog Spur VM and compiles the required NIB files

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/building/macos64ARMv8/squeak.cog.spur"

echo "Building Squeak Cog Spur VM for macOS ARM64..."
echo "================================================"

# Navigate to build directory
cd "$BUILD_DIR"

# Run the build
echo "Running VM build..."
./mvm -f

# Find ibtool
IBTOOL=""
if [ -x "/Applications/Xcode.app/Contents/Developer/usr/bin/ibtool" ]; then
    IBTOOL="/Applications/Xcode.app/Contents/Developer/usr/bin/ibtool"
elif [ -x "/usr/bin/ibtool" ]; then
    IBTOOL="/usr/bin/ibtool"
else
    echo "Error: ibtool not found. Please install Xcode."
    exit 1
fi

echo ""
echo "Compiling MainMenu.nib file..."
cd "$SCRIPT_DIR"
$IBTOOL --errors --warnings --notices \
    --module Squeak \
    --minimum-deployment-target 11.0 \
    --auto-activate-custom-fonts \
    --output-format human-readable-text \
    --compile "$BUILD_DIR/Squeak.app/Contents/Resources/English.lproj/MainMenu.nib" \
    "platforms/iOS/vm/English.lproj/Squeak-MainMenu.xib"

echo ""
echo "================================================"
echo "Build completed successfully!"
echo "Application: $BUILD_DIR/Squeak.app"
echo ""
echo "To run the app:"
echo "  open $BUILD_DIR/Squeak.app"

open $BUILD_DIR/Squeak.app