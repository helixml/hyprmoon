# HyprMoon Installation Guide

HyprMoon is Hyprland with integrated Moonlight streaming capabilities.

## Prerequisites

Before installing the HyprMoon deb package, you need to install the following dependencies:

### Core Wayland Dependencies
```bash
sudo apt-get update
sudo apt-get install -y \
    libwayland-server0 \
    libinput10 \
    libxkbcommon0 \
    libpixman-1-0 \
    libcairo2 \
    libdrm2 \
    libxcb1 \
    libpango-1.0-0 \
    libgdk-pixbuf2.0-0 \
    libseat1 \
    libudev1 \
    libsystemd0
```

### Hyprland-specific Dependencies
```bash
sudo apt-get install -y \
    libtomlplusplus3 \
    libzip4 \
    librsvg2-2 \
    libmagic1 \
    libhyprlang2 \
    libhyprutils1
```

### Moonlight Streaming Dependencies
```bash
sudo apt-get install -y \
    libcurl4 \
    libssl3t64 \
    libgstreamer1.0-0 \
    libgstreamer-plugins-base1.0-0 \
    libenet7 \
    libfmt9 \
    libboost-system1.83.0 \
    libboost-filesystem1.83.0
```

### GStreamer Plugins for Video Streaming
```bash
sudo apt-get install -y \
    gstreamer1.0-plugins-base \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-libav
```

## Installation

1. **Install all dependencies first:**
   ```bash
   # Run all the dependency installation commands above
   # OR install them all at once:
   sudo apt-get install -y \
       libwayland-server0 libinput10 libxkbcommon0 libpixman-1-0 \
       libcairo2 libdrm2 libxcb1 libpango-1.0-0 libgdk-pixbuf2.0-0 \
       libseat1 libudev1 libsystemd0 libtomlplusplus3 libzip4 \
       librsvg2-2 libmagic1 libhyprlang2 libhyprutils1 libcurl4 \
       libssl3t64 libgstreamer1.0-0 libgstreamer-plugins-base1.0-0 \
       libenet7 libfmt9 libboost-system1.83.0 libboost-filesystem1.83.0 \
       gstreamer1.0-plugins-base gstreamer1.0-plugins-good \
       gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav
   ```

2. **Install the HyprMoon package:**
   ```bash
   sudo dpkg -i hyprmoon_*.deb
   ```

3. **Fix any remaining dependency issues:**
   ```bash
   sudo apt-get install -f
   ```

## Package Information

- **Package Name:** hyprmoon
- **Conflicts with:** hyprland (this package replaces the standard Hyprland)
- **Provides:** x-window-manager
- **Binary location:** `/usr/bin/hyprmoon`
- **Compatibility symlink:** `/usr/bin/Hyprland-hyprmoon`

## What's Different from Standard Hyprland

HyprMoon includes:
- ✅ All standard Hyprland functionality
- ✅ Built-in Moonlight streaming server
- ✅ Network streaming capabilities via Moonlight protocol
- ✅ GStreamer-based video encoding pipeline
- ✅ Remote desktop access support

## Usage

After installation, you can use HyprMoon just like regular Hyprland:

```bash
# Start HyprMoon (same as Hyprland)
hyprmoon

# Or use the compatibility command
Hyprland-hyprmoon
```

The moonlight streaming capabilities are built-in and automatically available.

## Configuration

HyprMoon uses the same configuration format as Hyprland. Your existing `~/.config/hypr/hyprland.conf` will work unchanged.

Additional moonlight-specific configuration options are available (refer to the HyprMoon documentation).

## Troubleshooting

### Missing Dependencies
If you get dependency errors during installation:
```bash
sudo apt-get update
sudo apt-get install -f
```

### Package Conflicts
If you have regular Hyprland installed:
```bash
sudo apt-get remove hyprland
sudo dpkg -i hyprmoon_*.deb
```

### GPU Acceleration Issues
Make sure you have proper GPU drivers installed:
```bash
# For Intel/AMD
sudo apt-get install mesa-utils libgl1-mesa-dri

# For NVIDIA
sudo apt-get install nvidia-driver-XXX  # replace XXX with version
```

## Uninstallation

To remove HyprMoon and return to standard Hyprland:
```bash
sudo dpkg -r hyprmoon
sudo apt-get install hyprland  # reinstall standard Hyprland
```