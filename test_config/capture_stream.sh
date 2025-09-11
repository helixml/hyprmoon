#!/bin/bash
set -e

echo "Waiting for moonlight server..."

# Wait for moonlight server to be available
for i in {1..60}; do
  if curl -s http://hyprland-moonlight:47989/serverinfo > /dev/null 2>&1; then
    echo "Moonlight server detected!"
    break
  fi
  echo "Waiting for moonlight server... attempt $i"
  sleep 2
done

# Create output directory
mkdir -p /workspace/test_output

# Capture moonlight stream
echo "Capturing moonlight RTSP stream..."
timeout 15s ffmpeg -y \
  -f rtsp -rtsp_transport tcp \
  -i rtsp://hyprland-moonlight:48010 \
  -t 5 \
  -vf fps=1 \
  -frames:v 1 \
  /workspace/test_output/moonlight_screenshot.png \
  2>&1 | tee /workspace/test_output/ffmpeg_log.txt
echo "FFmpeg capture completed with exit code: $?"
  
echo "Capture attempt completed"
ls -la /workspace/test_output/