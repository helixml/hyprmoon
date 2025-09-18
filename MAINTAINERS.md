# HyprMoon Build System - Maintainer Guide

## Overview

HyprMoon uses a sophisticated incremental build system that minimizes iteration time for development while maintaining full .deb packaging capability.

## Build Modes

### 🔧 **Full Build**
Used for:
- First build in a clean environment
- When dependencies change in `debian/control`
- When `FORCE_CLEAN=1` is specified

**Process:**
1. Auto-increments version (step8.9.X → step8.9.X+1)
2. Runs `dpkg-buildpackage -us -uc -b` (full Debian packaging)
3. Generates complete .deb packages
4. Deploys .deb packages to helix container
5. Container installs packages normally

### 🚀 **Incremental Build**
Used for:
- Source code changes when build/ directory exists
- Fast iteration during development

**Process:**
1. Auto-increments version (step8.9.X → step8.9.X+1)
2. Runs direct `ninja` compilation (skips packaging)
3. Copies binary as `Hyprland-VERSION`
4. Creates `HYPRMOON_VERSION.txt` version file
5. Deploys binary to helix container
6. Container **clobbers** `/usr/bin/Hyprland` with new binary

## Build System Components

### `build.sh` (Host-side orchestrator)
- **Auto-version bumping**: Increments step8.9.X on every build
- **Concurrency protection**: Uses fixed container name `hyprmoon-builder`
- **Build cache management**: FORCE_CLEAN support with sudo
- **Smart deployment**: Timestamp comparison determines incremental vs full
- **Comprehensive metrics**: CSV logging with performance data

### `container-build.sh` (Container-side builder)
- **Dependency optimization**: Uses `mk-build-deps` without `--remove` flag
- **Incremental detection**: Checks for existing `build/.ninja_log`
- **Direct ninja builds**: Bypasses Debian packaging for speed
- **ccache integration**: Proper `/ccache` directory usage
- **Performance tracking**: Before/after metrics collection

### `debian/rules` (Packaging rules)
- **Vanilla configuration**: No special incremental logic
- **Build cache preservation**: `dh_clean -X build/` excludes build directory
- **Standard Debian compliance**: Works with both dpkg-buildpackage and direct ninja

### `Dockerfile.zed-agent-vnc` (Container deployment)
- **Dual deployment modes**: Standard .deb install OR binary clobbering
- **Version tracking**: `/HYPRMOON_VERSION.txt` instead of dpkg queries
- **Commented templates**: Incremental COPY sections enabled via sed
- **Cache optimization**: Go build mount caches for faster API builds

## Usage

### First Build (Required)
```bash
FORCE_CLEAN=1 ./build.sh
```

### Development Iteration
```bash
# Edit source code
./build.sh  # Automatically uses incremental mode
```

### Dependency Changes
```bash
# Edit debian/control to add dependencies
FORCE_CLEAN=1 ./build.sh  # Forces full rebuild
```

### Force Full Build
```bash
FORCE_CLEAN=1 ./build.sh
```

## Performance Monitoring

### Build Metrics (`build-metrics.csv`)
Tracks comprehensive build performance:
- Duration, ccache hit rates, ninja targets rebuilt
- Binary MD5 hash, git commit, changed files
- Build type (incremental vs full)

### Ninja Debug (`ninja-debug.log`)
Shows detailed ninja behavior:
- Why targets are being rebuilt
- Dependency tracking status
- Build cache effectiveness

### ccache Stats
Displayed before/after each build:
- Cache hit percentage
- Number of cached files
- Cache size utilization

## Troubleshooting

### "Incremental build requires existing .deb package"
**Solution:** Run `FORCE_CLEAN=1 ./build.sh` to create base .deb first

### Slow incremental builds (>2 minutes)
**Causes:**
- CMake reconfiguration happening (check ninja-debug.log)
- Low ccache hit rate (<80%)
- Many targets rebuilding unnecessarily

### Version mismatch in container
**Check:** `cat /HYPRMOON_VERSION.txt` in container vs build.sh version
**Solution:** Ensure proper auto-deployment completed

### Code changes not appearing
**Check:** Binary MD5 in CSV metrics - should change with code changes
**Solution:** Verify incremental build detected and ninja compilation ran

## Architecture Benefits

✅ **Fast iteration**: Incremental builds ~1-2 minutes vs 8+ minutes
✅ **Automatic versioning**: No manual version management needed
✅ **Flexible deployment**: Handles both packaging and binary updates
✅ **Cache optimization**: ccache + ninja + Docker layer caching
✅ **Development safety**: Auto-deployment with rollback via .deb packages
✅ **Performance visibility**: Comprehensive metrics and debugging

## Dependency Management

- **Container pre-loads**: Common dependencies in `hyprmoon-build-env`
- **Dynamic installation**: `mk-build-deps` handles new dependencies
- **Cache preservation**: Dummy packages prevent reinstallation
- **Manual control**: Edit `debian/control`, run `FORCE_CLEAN=1 ./build.sh`

This system enables rapid HyprMoon development with full Debian packaging compliance.