# HyprMoon Development Plan

## Development Context

### Directory Structure
- **Host System**: Ubuntu 24.04 LTS (noble) - does NOT have hyprland package
- **Container Environment**: Ubuntu 25.04 (plucky) - HAS hyprland 0.41.2+ds-1.3 in universe repo
- **New methodical repo**: `~/pm/hyprmoon/` (this directory)
- **Previous attempt**: `~/pm/Hyprland-wlroots/` (big-bang approach, caused grey screen)
- **Helix container environment**: `/home/luke/pm/helix/` (test environment)
- **Ubuntu source**: `~/pm/hyprmoon/hyprland-0.41.2+ds/` (baseline from Ubuntu 25.04 deb-src)

### Version Documentation
- **Ubuntu 25.04 (plucky)**: Source distribution for container builds
- **Hyprland 0.41.2+ds-1.3**: Exact Ubuntu package version from universe repo
  - Source package: `hyprland_0.41.2+ds-1.3.dsc`
  - Generated packages: `hyprland_0.41.2+ds-1.3_amd64.deb` (2MB), `hyprland-backgrounds_0.41.2+ds-1.3_all.deb` (47MB), `hyprland-dev_0.41.2+ds-1.3_amd64.deb` (176KB)
- **Build System**: CMake 3.31.6 with Ninja backend (not meson as initially assumed)
- **CMake auto-includes**: Uses `file(GLOB_RECURSE SRCFILES "src/*.cpp")` - all .cpp files automatically included
- **Ubuntu Patches Applied**: 5 debian patches already in source:
  - 001-use-bash-in-makefile.patch
  - 002-use-system-udis86.patch
  - 003-use-system-hyprland-protocols.patch
  - 004-fix-hyprland-symlink.patch
  - 005-add-fortify-flags-for-subprojects.patch

### Previous Findings
1. **Grey Screen Issue**: Current HyprMoon shows grey screen in VNC instead of black
2. **Screencopy Works**: `zwlr_screencopy_manager_v1` protocol is functional
3. **Compositor Running**: Hyprland starts, manages windows (Chrome launched successfully)
4. **VNC Connected**: wayvnc connects and captures, but wrong colors
5. **Ubuntu Patches Applied**: 5 debian patches already applied to source

### Key Development Lessons
- **Always run builds in foreground**: Never use `&` or background mode for build commands, be patient
- **Build caches are critical**: Without ccache/meson cache, iteration takes too long
- **Test after every change**: Big-bang approaches are impossible to debug
- **Use exact Ubuntu source**: Don't deviate from what Ubuntu ships
- **Container build caches matter**: Use BuildKit mount caches for Docker builds
- **Git commit discipline**: Make commits after every substantial change
- **Phase milestone commits**: ALWAYS commit when reaching phase milestones
- **Manual testing required**: Human verification at every step, no automation
- **CRITICAL: Always start helix container before manual testing**: MUST check `docker ps | grep helix` and start container if needed before asking user to test via VNC
- **MANDATORY 60-SECOND BUILD MONITORING**: ALWAYS monitor builds every 60 seconds using BashOutput tool until completion - NEVER start a build and forget about it
- **NEVER GIVE UP ON LONG BUILDS**: ALWAYS wait patiently for builds to complete, no matter how long they take - builds can take 10+ minutes, be patient and keep monitoring every 60 seconds
- **CRITICAL: ALWAYS exec sleep 60**: When waiting 60 seconds for build monitoring, MUST use `sleep 60` command - DO NOT just wait passively
- **NEVER USE --no-cache**: NEVER EVER use --no-cache flags with Docker builds - we trust Docker's caching system completely
- **DOCKERFILE FILENAME UPDATES CRITICAL**: When moving from Step N to Step N+1, you MUST update the Dockerfile COPY lines to reference the new Step filenames BEFORE running docker build. Docker caching works correctly - if you update the Dockerfile after build starts, it uses cached layers with old filenames.

## Goal
Build HyprMoon (Hyprland + Moonlight integration) systematically from Ubuntu's exact source, adding features incrementally while maintaining VNC connectivity.

## CRITICAL: Use Wolf Moonlight Implementation
**DO NOT reimplement moonlight protocol from scratch!** Use the battle-tested implementation from `~/pm/wolf`:
- Wolf has a working, tested moonlight protocol implementation
- Wolf also has a GStreamer stack for WebRTC implementation
- Only write glue code between Wolf's moonlight code and Hyprland
- Copy (using `cp`) the core protocol code from Wolf
- Copy (using `cp`) the GStreamer/streaming code from Wolf
- We only need to implement the integration layer and PIN authentication server
- **CRITICAL REFERENCE**: Use existing HyprMoon implementation in `/home/luke/pm/Hyprland-wlroots/` for reference
- The existing HyprMoon already has this work done - use it as a guide for integration patterns
- **For Step 3 specifically**: Reference how global managers are integrated in the previous implementation

## Current Problem
- HyprMoon container shows grey screen in VNC (should be black like vanilla Hyprland)
- Need to isolate which modification broke the rendering/capture
- Original approach was too big-bang, hard to debug

## Methodical Approach

### Phase 1: Baseline Setup ✅ COMPLETE
1. **Ubuntu Source**: Use exact Ubuntu 25.04 Hyprland 0.41.2+ds-1.3 source ✅
2. **Apply Ubuntu Patches**: ✅
   - 001-use-bash-in-makefile.patch
   - 002-use-system-udis86.patch
   - 003-use-system-hyprland-protocols.patch
   - 004-fix-hyprland-symlink.patch
   - 005-add-fortify-flags-for-subprojects.patch
3. **Build Cache**: Set up proper Debian build environment with ccache ✅
4. **Build Raw Ubuntu Package**: Build vanilla Ubuntu Hyprland deb (no changes) ✅
5. **Test Raw Baseline**: Deploy to helix and verify VNC connectivity ✅
6. **Commit Baseline**: Git commit the working vanilla state ✅
   - **Git commit**: 249b009 "Step 1 Complete: Clean Ubuntu Hyprland baseline"

### Phase 2: Incremental Modifications
Add HyprMoon features one by one, testing VNC after each:

#### Step 1: Build System Integration (Low Risk) - COMPLETED ✅
- Add meson option `with_moonlight` (disabled initially) ✅
- Add moonlight subdirectory to build system ✅
- Add minimal moonlight manager stub ✅
- **Test**: VNC should still show black screen, build works ✅

#### Step 2: Core Protocol Infrastructure (Medium Risk) - COMPLETED ✅
- Add moonlight protocol crypto and data structures ✅
- Add RTSP and RTP protocol handling ✅
- Add configuration system (TOML) ✅
- **Test**: VNC should still work, moonlight subsystem compiles ✅

#### Step 3: Global Manager Integration ✅ COMPLETE
- Add MoonlightManager to global scope ✅
- Enable `with_moonlight` option by default ✅
- Add manager lifecycle to compositor ✅
- **Test**: VNC still works, moonlight manager initializes ✅
- **Git commit**: 8a2e5ed "Step 3 Complete: Global Manager Integration" ✅
- **Result**: SUCCESSFUL - VNC connectivity maintained, manager integrated properly

#### Step 4: Frame Capture Integration ✅ COMPLETE
- Add frame capture hooks to renderer ✅
- Add `g_pMoonlightManager->onFrameReady()` calls ✅
- Implement minimal frame capture method with logging ✅
- Fix Debian packaging configuration ✅
- **Build**: Generated `hyprmoon_0.41.2+ds-1.3+step5_amd64.deb` successfully ✅
- **Test**: VNC testing SUCCESSFUL - frame capture working ✅
- **Git commit**: b17506a "Step 5 Complete: Frame Capture Integration" ✅
- **Result**: SUCCESSFUL - Frame capture hooks integrated, VNC connectivity maintained

#### Step 5: Stub Infrastructure ✅ COMPLETE
- Add stub REST API server infrastructure ✅
- Add stub streaming manager with GStreamer ✅
- Add stub input management ✅
- Automatic REST API startup in MoonlightManager ✅
- **Build**: Generated `hyprmoon_0.41.2+ds-1.3+step8_amd64.deb` successfully ✅
- **Test**: VNC testing SUCCESSFUL - all infrastructure initializes ✅
- **Git commit**: 93c1c99 "Step 8 Complete: REST API Activation" ✅
- **Result**: SUCCESSFUL - All stub infrastructure in place, ready for Wolf implementation

#### Step 6: Copy Wolf Core Protocol Implementation ✅ COMPLETE
- Copy Wolf moonlight protocol from `/home/luke/pm/Hyprland-wlroots/src/moonlight/protocol/` ✅
- Copy Wolf crypto implementation from `/home/luke/pm/Hyprland-wlroots/src/moonlight/crypto/` ✅
- Copy Wolf core components from `/home/luke/pm/Hyprland-wlroots/src/moonlight/core/` ✅
- Replace stub implementations with real Wolf code ✅
- Added Wolf dependencies to meson.build: boost, gstreamer, enet, fmt, openssl, icu, libpci, curl ✅
- **Build**: Generated `hyprmoon_0.41.2+ds-1.3+step6-wolf_amd64.deb` successfully ✅
- **Git commit**: Committed in Step 6 work ✅
- **Result**: SUCCESSFUL - Real Wolf protocol code integrated, ready for REST API and streaming

#### Step 7: Copy Wolf REST API and PIN Management ✅ COMPLETE
- Copy Wolf REST API from `/home/luke/pm/Hyprland-wlroots/src/moonlight/rest/` ✅
- Copy Wolf state management from `/home/luke/pm/Hyprland-wlroots/src/moonlight/state/` ✅
- Replace RestServerStub with real Wolf REST implementation ✅
- Implemented HTTP and HTTPS servers using Wolf's SimpleWeb framework ✅
- Added Wolf AppState initialization for REST endpoints and state management ✅
- Added self-signed certificate generation for HTTPS endpoints ✅
- **Build**: Generated `hyprmoon_0.41.2+ds-1.3+step7_amd64.deb` successfully ✅
- **Test**: VNC testing SUCCESSFUL - Wolf REST API deployed and ready ✅
- **Git commit**: Ready for Step 7.1 with corrected versioning ✅
- **Result**: SUCCESSFUL - Real Wolf REST API integrated, NO MORE STUBS!

**NOTE**: Step 7 was actually built as `step7` not `step7-rest` in the generated packages. Ready for version 7.1 with corrected naming.

#### Step 8: Copy Wolf Streaming Infrastructure (PENDING - COPY FROM Hyprland-wlroots)
- Copy Wolf streaming from `/home/luke/pm/Hyprland-wlroots/src/moonlight/streaming/`
- Copy Wolf GStreamer plugins from `/home/luke/pm/Hyprland-wlroots/src/moonlight/gst-plugin/`
- Copy Wolf control system from `/home/luke/pm/Hyprland-wlroots/src/moonlight/control/`
- Replace streaming stubs with real Wolf implementation
- **Test**: Moonlight streaming should work end-to-end

#### Step 9: Copy Wolf WebRTC and Voice Support (PENDING - COPY FROM Hyprland-wlroots)
- Copy Wolf WebRTC from `/home/luke/pm/Hyprland-wlroots/src/moonlight/webrtc/`
- Copy Wolf voice system from `/home/luke/pm/Hyprland-wlroots/src/moonlight/voice/`
- Copy Wolf input management from `/home/luke/pm/Hyprland-wlroots/src/moonlight/platforms/`
- **Test**: Browser streaming and voice commands should work

### Phase 3: Build & Deployment Strategy

#### Fast Iteration Options:
1. **Containerized Build**: Mount source into build container with ccache
2. **Incremental Debs**: Build .deb packages, copy to helix container
3. **Direct Binary**: Build binary, copy to running container for testing

#### Build Cache Requirements:
- ccache for C++ compilation
- Meson build cache
- Debian package cache
- Container layer cache

## RELIABLE BUILD AND DEPLOYMENT PROCESS (PROVEN METHOD)

### Consolidated Build Process (MANDATORY FOR ALL STEPS)

This is the proven process for building AND deploying HyprMoon deb packages. **CRITICAL: ALWAYS complete ALL steps before testing to ensure we test the latest code:**

#### 1. Build Process
```bash
# Navigate to hyprmoon directory
cd /home/luke/pm/hyprmoon

# Run the consolidated build script
./build.sh

# The build script handles:
# - Bind mounts with hyprmoon-build-env container
# - Timestamped build logging automatically
# - Dependency installation and caching
# - deb package generation with proper naming
# - Clear success/failure feedback
```

#### 2. Deployment Process (MANDATORY BEFORE TESTING)
```bash
# Copy generated deb files to helix directory
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

# Verify container is running
docker ps | grep helix

# Wait 30-60 seconds for full startup before VNC testing
```

**CRITICAL: NEVER test without completing the full deployment process!**

### Why This Method Works:
- **Bind Mounts**: Source code mounted directly, no Docker layer copying
- **Dependency Caching**: Build container has all deps pre-installed
- **Ubuntu 25.04**: Only place with required hyprland dev packages
- **Incremental**: No full Docker rebuild on source changes
- **Fast**: ~5-10 minutes vs 30+ minutes for full Docker builds

### Prerequisites:
1. Build environment container must exist: `hyprmoon-build-env`
2. All source changes committed to hyprmoon directory
3. Helix container environment ready for testing

### CRITICAL BUILD LOG REQUIREMENTS:
- **ALWAYS capture build output to timestamped log files**: Use `2>&1 | tee build-$(date +%s).log` for complete error capture
- **NEVER use --no-cache flags**: We DO want build caching for speed and efficiency
- **MANDATORY 60-SECOND BUILD MONITORING**: ALWAYS monitor builds every 60 seconds using BashOutput tool - CHECK AFTER EACH 60-SECOND WAIT
- **LOOP FOREVER until build completion**: NEVER give up - keep checking every 60 seconds until success or failure
- **NEVER GIVE UP ON LONG BUILDS**: Builds can take 10+ minutes, be patient and keep monitoring
- **ALWAYS run builds in foreground**: Never use `&` or background mode for build commands
- **When builds fail**: Inspect the complete log file for full error details, never rely on truncated output
- **Find latest build log**: Use `ls -lt build-*.log | head -1` to reliably identify the most recent build log

### Phase 4: Testing Protocol
For each step:
1. Build new deb/binary
2. **CRITICAL: Update helix Dockerfile to install the new deb package**
   - Copy deb to `/home/luke/pm/helix/` directory
   - Modify `/home/luke/pm/helix/Dockerfile.zed-agent-vnc` to install the deb
   - Rebuild helix container: `docker build -f Dockerfile.zed-agent-vnc -t helix-zed-runner .`
   - Restart container if needed
3. Test VNC connection manually (port 5901)
4. Verify expected behavior (working helix desktop with GPU acceleration)

## Comprehensive HyprMoon Modifications

### Major Components Added:
1. **Moonlight Protocol System** (`src/moonlight/protocol/`):
   - Full Moonlight protocol implementation with crypto support
   - RTSP protocol handling
   - FEC (Forward Error Correction)
   - Custom data structures and control protocols

2. **Streaming Infrastructure** (`src/moonlight/streaming/`):
   - RTP/UDP streaming implementation
   - RTSP command handling
   - Network optimizations and UDP ping

3. **GStreamer Integration** (`src/moonlight/gst-plugin/`):
   - Custom GStreamer plugins for Moonlight RTP payloads
   - Video and audio frame processing
   - Direct Hyprland frame source integration
   - Hardware-accelerated encoding pipelines

4. **WebRTC Support** (`src/moonlight/webrtc/`):
   - WebRTC manager for browser-based streaming
   - Real-time communication protocol
   - Client example implementation

5. **Voice Input System** (`src/moonlight/voice/`):
   - Whisper integration for voice recognition
   - Voice API endpoints
   - AI-powered voice commands

6. **Input Management** (`src/moonlight/platforms/`, `src/moonlight/control/`):
   - Virtual input device creation (uinput)
   - Keyboard, mouse, touchscreen, gamepad support
   - Input routing and forwarding from clients
   - Hardware abstraction for Linux platforms

7. **REST API Server** (`src/moonlight/rest/`):
   - HTTPS endpoint handling
   - Pairing and session management
   - Authentication and security
   - HTML interfaces for configuration

8. **Configuration System** (`src/moonlight/state/`):
   - TOML-based configuration
   - Session state management
   - Default configuration templates
   - Serialized config handling

9. **Wolf Implementation** (`src/moonlight/wolf-impl/`):
   - Wolf Moonlight server backend
   - Performance-optimized streaming

### Core Hyprland Integration Points:
1. **Renderer Integration** (`src/render/Renderer.cpp`):
   - Frame capture hooks: `g_pMoonlightManager->onFrameReady()`
   - Direct buffer access for streaming
   - **This is the integration that likely broke VNC capture**

2. **Build System Integration**:
   - Meson option: `with_moonlight` (enabled by default)
   - Conditional compilation of entire moonlight subsystem
   - Additional dependencies and libraries

3. **Global Manager** (`src/moonlight/managers/MoonlightManager.cpp`):
   - Central coordinator: `g_pMoonlightManager`
   - Lifecycle management
   - Cross-component communication

### Additional Infrastructure:
- **Crypto Support**: Full certificate and encryption handling
- **Hardware Abstraction**: GPU encoding and decoding
- **Network Stack**: Custom UDP/TCP handling optimized for streaming
- **Configuration Management**: Dynamic config reloading
- **Session Management**: Multi-client support and pairing

### Corrected Understanding:
- **VNC and Moonlight are parallel**: Both provide desktop access, not nested
- **The grey screen issue**: Likely caused by renderer integration changes
- **No "green screen windows"**: Moonlight clients stream the entire desktop
- **WebRTC and voice**: Major missing pieces from original analysis

## Success Criteria
- VNC shows black screen with vanilla Ubuntu baseline
- VNC continues to work throughout incremental integration
- Moonlight streaming functionality works (tested manually)
- WebRTC streaming works (browser-based access)
- Voice input system functional
- No crashes or stability issues
- Performance acceptable for both VNC and streaming

## Implementation Notes
- Use Ubuntu's exact source as baseline
- Apply changes incrementally with git commits
- Test after each change
- Use build cache for fast iteration
- Document what breaks at each step
