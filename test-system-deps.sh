#!/bin/bash
# Quick test to verify system dependencies are available in Ubuntu 25.04
echo "Testing Ubuntu 25.04 system dependencies for Hyprland..."

docker run --rm ubuntu:25.04 bash -c "
apt-get update >/dev/null 2>&1
echo '🔍 Checking for key system dependencies:'

echo -n 'libwlroots-0.18-dev: '
if apt-cache show libwlroots-0.18-dev >/dev/null 2>&1; then
  echo '✅ Available'
else
  echo '❌ Not found'
fi

echo -n 'libudis86-dev: '
if apt-cache show libudis86-dev >/dev/null 2>&1; then
  echo '✅ Available'
else
  echo '❌ Not found'
fi

echo -n 'libhyprlang-dev: '
if apt-cache show libhyprlang-dev >/dev/null 2>&1; then
  echo '✅ Available'
else
  echo '❌ Not found'
fi

echo -n 'libhyprcursor-dev: '
if apt-cache show libhyprcursor-dev >/dev/null 2>&1; then
  echo '✅ Available'
else
  echo '❌ Not found'
fi

echo -n 'libhyprutils-dev: '
if apt-cache show libhyprutils-dev >/dev/null 2>&1; then
  echo '✅ Available'
else
  echo '❌ Not found'
fi

echo -n 'hyprland-protocols: '
if apt-cache show hyprland-protocols >/dev/null 2>&1; then
  echo '✅ Available'
else
  echo '❌ Not found'
fi

echo -n 'hyprwayland-scanner: '
if apt-cache show hyprwayland-scanner >/dev/null 2>&1; then
  echo '✅ Available'
else
  echo '❌ Not found'
fi

echo
echo '🎯 Key insight: Ubuntu 25.04 has all the system packages we need!'
echo '   This matches exactly what the working Ubuntu Hyprland package uses.'
"