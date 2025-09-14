#!/bin/bash
set -euo pipefail

echo "🔨 Building Raw Ubuntu Hyprland Package (No Modifications)"
echo "=========================================================="

# Create output directory for deb packages
OUTPUT_DIR="$(pwd)/deb-output"
mkdir -p "$OUTPUT_DIR"

echo "📁 Output directory: $OUTPUT_DIR"

# Build the deb builder image
echo "🏗️  Building raw Hyprland deb package builder image..."
docker build -f Dockerfile.raw-builder -t hyprmoon-raw-builder .

# Run the builder and copy packages to bind mount
echo "📦 Building raw Ubuntu Hyprland packages..."
docker run --rm \
    -v "$OUTPUT_DIR:/output-bind" \
    hyprmoon-raw-builder

# Show results
echo ""
echo "🎉 Raw Ubuntu Hyprland Package Build Complete!"
echo "=============================================="
echo "📁 Packages available in: $OUTPUT_DIR"
echo ""

if [ -d "$OUTPUT_DIR" ] && [ -n "$(ls -A "$OUTPUT_DIR" 2>/dev/null)" ]; then
    echo "📦 Built packages:"
    ls -la "$OUTPUT_DIR"
    echo ""

    # Show package details
    for deb in "$OUTPUT_DIR"/*.deb; do
        if [ -f "$deb" ]; then
            echo "📋 Package info for $(basename "$deb"):"
            dpkg-deb -I "$deb" | head -20
            echo ""
        fi
    done

    echo "🚀 To install in your helix setup:"
    echo "   sudo dpkg -i $OUTPUT_DIR/hyprland_*.deb"
    echo "   sudo apt-get install -f  # if there are dependency issues"
    echo ""
    echo "📋 This should be identical to Ubuntu's official hyprland package"

else
    echo "❌ No packages were generated. Check the build logs above."
    exit 1
fi