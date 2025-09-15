# Claude Rules for HyprMoon Development

This file contains critical development guidelines and context that MUST be followed at all times during HyprMoon development.

## CRITICAL: Always Reference design.md

Before taking any action, ALWAYS read and follow the current `/home/luke/pm/hyprmoon/design.md` file. This file contains:
- Current development context and directory structure
- Key development lessons learned
- Previous findings and known issues
- Current problem description
- Methodical approach phases and steps
- Success criteria

## Key Development Rules (from design.md)

### ALWAYS Follow These Rules:
1. **Always run builds in foreground**: Never use `&` or background mode for build commands, be patient
2. **Build caches are critical**: Without ccache/meson cache, iteration takes too long
3. **Test after every change**: Big-bang approaches are impossible to debug
4. **Use exact Ubuntu source**: Don't deviate from what Ubuntu ships
5. **Container build caches matter**: Use BuildKit mount caches for Docker builds
6. **Git commit discipline**: Make commits after every substantial change
7. **Phase milestone commits**: ALWAYS commit when reaching phase milestones
8. **Manual testing required**: Human verification at every step, no automation
9. **CRITICAL: Always start helix container before manual testing**: MUST check `docker ps | grep helix` and start container if needed before asking user to test via VNC
10. **MANDATORY 60-SECOND BUILD MONITORING**: ALWAYS monitor builds every 60 seconds using BashOutput tool until completion - NEVER start a build and forget about it
11. **NEVER GIVE UP ON LONG BUILDS**: ALWAYS wait patiently for builds to complete, no matter how long they take - builds can take 10+ minutes, be patient and keep monitoring every 60 seconds

### Current Development Context:
- **New methodical repo**: `~/pm/hyprmoon/` (this directory)
- **Previous attempt**: `~/pm/Hyprland-wlroots/` (big-bang approach, caused grey screen)
- **Helix container environment**: `/home/luke/pm/helix/` (test environment)
- **Ubuntu source**: `~/pm/hyprmoon/hyprland-0.41.2+ds/` (baseline)

### Current Problem:
- HyprMoon container shows grey screen in VNC instead of working helix desktop
- Need to isolate which modification broke the rendering/capture
- Using methodical incremental approach instead of big-bang

### Current Status:
Phase 1 Complete: Raw Ubuntu package built and ready for testing
Next: Test baseline, then incremental moonlight integration

## CONSOLIDATED BUILD AND DEPLOYMENT PROCESS (CRITICAL REFERENCE)

**ALWAYS use this EXACT process for building AND deploying HyprMoon deb packages:**

### Step 1: Build the deb packages
```bash
# Navigate to hyprmoon directory
cd /home/luke/pm/hyprmoon

# Run the consolidated build script (handles everything)
./build.sh

# This script:
# - Uses bind mounts with hyprmoon-build-env container
# - Captures timestamped build output automatically
# - Runs container-build.sh inside the container
# - Provides clear success/failure feedback
# - Generates properly named deb files
```

### Step 2: Deploy to helix container (MANDATORY BEFORE TESTING)
```bash
# Copy deb files to helix directory
cp hyprmoon_*.deb hyprland-backgrounds_*.deb /home/luke/pm/helix/

# Navigate to helix directory
cd /home/luke/pm/helix

# Update Dockerfile.zed-agent-vnc with EXACT deb filenames
# (Update COPY lines to match the generated deb filenames)

# Rebuild helix container with new debs
docker compose -f docker-compose.dev.yaml build zed-runner

# CRITICAL: Recreate container to get clean state from image
# NEVER just restart - that preserves modified container state!
docker compose -f docker-compose.dev.yaml down zed-runner
docker compose -f docker-compose.dev.yaml up -d zed-runner
```

### Step 3: Verify deployment before testing
```bash
# Check container is running
docker ps | grep helix

# Wait for container to fully start (give it 30-60 seconds)
# Then connect via VNC on port 5901 for testing
```

**CRITICAL: NEVER test without completing ALL steps above!**

**Why This Process is Mandatory:**
- ✅ **Uses bind mounts**: No Docker layer copying overhead
- ✅ **Ubuntu 25.04 container**: Only environment with required dev packages
- ✅ **Dependency caching**: Build container pre-loaded with all deps
- ✅ **Fast iteration**: 5-10 minutes vs 30+ for full rebuilds
- ✅ **Proven reliable**: Successfully builds with correct versioning
- ✅ **Always latest**: Ensures we test the actual code we just built
- ✅ **Incremental safety**: Prevents testing stale versions that invalidate our methodology

**CRITICAL BUILD LOG REQUIREMENTS:**
- ALWAYS capture build output to timestamped log files: `command 2>&1 | tee build-$(date +%s).log`
- NEVER use --no-cache flags - we DO want build caching for speed
- NEVER EVER use --no-cache with docker builds - we trust Docker's caching system completely
- **CRITICAL: Monitor CONTAINER logs for actual compiler errors**: The outer build-*.log only shows package management
- **MUST monitor container-build-*.log files**: These contain the actual compilation output and error details
- MANDATORY 60-second build monitoring with BashOutput tool - CHECK AFTER EACH 60-SECOND WAIT
- LOOP FOREVER until the build is finished (success or failure) - NEVER give up
- NEVER background builds - always foreground and patient monitoring
- Builds can take 10+ minutes - never give up on long builds
- When builds fail, inspect the complete CONTAINER log file for compiler errors
- Use `find . -name "container-build-*.log" | sort | tail -1` to find the latest container build log
- **Check both logs**: outer build-*.log for overall status, container-build-*.log for compilation errors

## MUST ALWAYS DO BEFORE MANUAL TESTING:
1. Check if helix container is running: `docker ps | grep helix`
2. If not running, start it before asking user to test
3. Provide VNC connection details (port 5901)
4. Only then ask user to manually test via VNC

## REFERENCE: Previous HyprMoon Implementation
When implementing integration patterns (especially global managers and build system integration), ALWAYS reference the working implementation in:
- **Previous attempt**: `~/pm/Hyprland-wlroots/` (big-bang approach, caused grey screen but did compile)
- This directory contains a working global manager integration example
- Use it as a reference for patterns, includes, and integration points
- Specifically useful for Step 3: Global Manager Integration

This file must be kept up to date with any critical lessons learned during development.