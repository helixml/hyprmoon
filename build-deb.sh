#!/bin/bash
set -euo pipefail

echo "🔨 Building HyprMoon Debian Package"
echo "=================================="

# Create output directory for deb packages
OUTPUT_DIR="$(pwd)/deb-output"
mkdir -p "$OUTPUT_DIR"

echo "📁 Output directory: $OUTPUT_DIR"

# Build the deb builder image
echo "🏗️  Building deb package builder image..."
docker build -f Dockerfile.deb-builder -t hyprmoon-deb-builder .

# Run the builder and copy packages to bind mount
echo "📦 Building debian packages..."
docker run --rm \
    -v "$OUTPUT_DIR:/output-bind" \
    hyprmoon-deb-builder

# Show results
echo ""
echo "🎉 HyprMoon Debian Package Build Complete!"
echo "=========================================="
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
            echo "📄 Package contents:"
            dpkg-deb -c "$deb" | head -10
            echo "   ... (truncated)"
            echo ""
        fi
    done
    
    echo "🚀 To install in your helix setup:"
    echo "   sudo dpkg -i $OUTPUT_DIR/hyprmoon_*.deb"
    echo "   sudo apt-get install -f  # if there are dependency issues"
    echo ""
    echo "📋 This will replace your system hyprland package with HyprMoon"
    echo "   (includes moonlight streaming integration)"
    
else
    echo "❌ No packages were generated. Check the build logs above."
    exit 1
fi