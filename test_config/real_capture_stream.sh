#!/bin/bash
set -e

echo "=== Real End-to-End HyprMoon Test ==="
echo "Waiting for actual moonlight server on hyprland-moonlight..."

# Wait for moonlight server to be available
for i in {1..60}; do
  if curl -s http://hyprland-moonlight:47989/serverinfo > /dev/null 2>&1; then
    echo "✅ Moonlight server detected on port 47989!"
    break
  fi
  echo "⏳ Waiting for moonlight server... attempt $i/60"
  sleep 2
done

# Check if RTSP port is responding
echo "🔍 Checking RTSP server availability..."
for i in {1..30}; do
  if nc -z hyprland-moonlight 48010 2>/dev/null; then
    echo "✅ RTSP server detected on port 48010!"
    break
  fi
  echo "⏳ Waiting for RTSP server... attempt $i/30"
  sleep 2
done

# Create output directory
mkdir -p /workspace/test_output

echo "📡 Testing moonlight HTTP API..."
curl -v http://hyprland-moonlight:47989/serverinfo 2>&1 | tee /workspace/test_output/http_response.txt || echo "HTTP request failed"

echo "📺 Attempting to capture actual RTSP moonlight stream..."

# Try to capture the RTSP stream with more aggressive settings
timeout 30s ffmpeg -y \
  -rtsp_transport tcp \
  -i rtsp://hyprland-moonlight:48010/moonlight \
  -t 10 \
  -r 1 \
  -vf "scale=400:300" \
  -frames:v 5 \
  /workspace/test_output/moonlight_capture_%03d.png \
  -f mp4 \
  /workspace/test_output/moonlight_stream.mp4 \
  2>&1 | tee /workspace/test_output/ffmpeg_capture.log

echo "🔬 Analyzing captured frames..."
# Check if we actually captured anything
for f in /workspace/test_output/moonlight_capture_*.png; do
  if [ -f "$f" ]; then
    echo "📸 Found frame: $f"
    # Use ImageMagick to analyze pixel colors
    if command -v identify >/dev/null 2>&1; then
      identify -verbose "$f" | grep -i "green\|color" || echo "No color analysis available"
    fi
    # Check file size
    size=$(stat -c%s "$f" 2>/dev/null || echo "0")
    echo "   File size: $size bytes"
    if [ "$size" -gt 1000 ]; then
      echo "✅ Frame appears to contain real data (>1KB)"
    else
      echo "❌ Frame is too small, likely empty"
    fi
  fi
done

echo "📊 Capture Summary:"
ls -la /workspace/test_output/
echo ""

# Try to get a screenshot directly from the running Hyprland
echo "📸 Attempting direct screenshot via Hyprland..."
timeout 10s hyprctl screenshot /workspace/test_output/hyprland_direct.png 2>&1 || echo "Direct screenshot failed"

echo "✅ Real capture attempt completed!"
echo "Check /workspace/test_output/ for results"