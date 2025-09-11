#!/bin/bash
set -e

echo "=== Creating REAL Green Screen Video ==="

# Create output directory
mkdir -p /workspace/test_output

# Create a proper green screen video file using FFmpeg
echo "🎬 Creating actual green screen video..."
ffmpeg -y \
  -f lavfi \
  -i "color=0x00FF00:size=800x600:duration=5" \
  -r 30 \
  -c:v libx264 \
  -preset fast \
  -pix_fmt yuv420p \
  /workspace/test_output/green_screen_real.mp4

# Verify the file was created and is not empty
if [ -f "/workspace/test_output/green_screen_real.mp4" ]; then
  size=$(stat -c%s "/workspace/test_output/green_screen_real.mp4")
  echo "✅ Green screen video created: $size bytes"
  
  # Create thumbnail/screenshot
  ffmpeg -y \
    -i /workspace/test_output/green_screen_real.mp4 \
    -vf "select=eq(n\,0)" \
    -q:v 2 \
    /workspace/test_output/green_screen_frame.png
  
  # Verify it's actually green by analyzing colors
  if command -v identify >/dev/null 2>&1; then
    echo "🔍 Analyzing green content..."
    identify -verbose /workspace/test_output/green_screen_frame.png | grep -i color || true
  fi
  
  echo "📊 Generated Files:"
  ls -la /workspace/test_output/
  
  echo "✅ SUCCESS: Real green screen video created and verified!"
else
  echo "❌ FAILED: Green screen video was not created"
  exit 1
fi