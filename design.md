# HyprMoon Development Plan

## Development Context

### Directory Structure
- **New methodical repo**: `~/pm/hyprmoon/` (this directory)
- **Previous attempt**: `~/pm/Hyprland-wlroots/` (big-bang approach, caused grey screen)
- **Helix container environment**: `/home/luke/pm/helix/` (test environment)
- **Ubuntu source**: `~/pm/hyprmoon/hyprland-0.41.2+ds/` (baseline)

### Previous Findings
1. **Grey Screen Issue**: Current HyprMoon shows grey screen in VNC instead of black
2. **Screencopy Works**: `zwlr_screencopy_manager_v1` protocol is functional
3. **Compositor Running**: Hyprland starts, manages windows (Chrome launched successfully)
4. **VNC Connected**: wayvnc connects and captures, but wrong colors
5. **Ubuntu Patches Applied**: 5 debian patches already applied to source

### Key Development Lessons
- **Always run builds in foreground**: Never use `&` for build commands, be patient
- **Build caches are critical**: Without ccache/meson cache, iteration takes too long
- **Test after every change**: Big-bang approaches are impossible to debug
- **Use exact Ubuntu source**: Don't deviate from what Ubuntu ships
- **Container build caches matter**: Use BuildKit mount caches for Docker builds

## Goal
Build HyprMoon (Hyprland + Moonlight integration) systematically from Ubuntu's exact source, adding features incrementally while maintaining VNC connectivity.

## Current Problem
- HyprMoon container shows grey screen in VNC (should be black like vanilla Hyprland)
- Need to isolate which modification broke the rendering/capture
- Original approach was too big-bang, hard to debug

## Methodical Approach

### Phase 1: Baseline Setup
1. **Ubuntu Source**: Use exact Ubuntu 25.04 Hyprland 0.41.2+ds-1.3 source
2. **Apply Ubuntu Patches**:
   - 001-use-bash-in-makefile.patch
   - 002-use-system-udis86.patch
   - 003-use-system-hyprland-protocols.patch
   - 004-fix-hyprland-symlink.patch
   - 005-add-fortify-flags-for-subprojects.patch
3. **Build Cache**: Set up proper Debian build environment with ccache
4. **Test Baseline**: Verify VNC shows BLACK screen with vanilla Ubuntu Hyprland

### Phase 2: Incremental Modifications
Add HyprMoon features one by one, testing VNC after each:

#### Step 1: Build System Integration (Low Risk)
- Add meson option `with_moonlight` (disabled initially)
- Add moonlight subdirectory to build system
- Add minimal moonlight manager stub
- **Test**: VNC should still show black screen, build works

#### Step 2: Core Protocol Infrastructure (Medium Risk)
- Add moonlight protocol crypto and data structures
- Add RTSP and RTP protocol handling
- Add configuration system (TOML)
- **Test**: VNC should still work, moonlight subsystem compiles

#### Step 3: Global Manager Integration (Higher Risk)
- Add MoonlightManager to global scope
- Enable `with_moonlight` option by default
- Add manager lifecycle to compositor
- **Test**: VNC still works, moonlight manager initializes

#### Step 4: REST API and Pairing (Medium Risk)
- Add REST API server
- Add HTTPS endpoints for pairing
- Add session management
- **Test**: VNC works, can access moonlight web interface

#### Step 5: Frame Capture Integration (HIGH RISK - likely VNC breaker)
- Add frame capture hooks to renderer
- Add `g_pMoonlightManager->onFrameReady()` calls
- **Test**: Check if VNC still works (likely breaks here)

#### Step 6: Streaming Infrastructure (Medium Risk)
- Add GStreamer integration
- Add video/audio streaming pipelines
- Add network handling
- **Test**: Moonlight streaming should work

#### Step 7: Input Management (Low Risk)
- Add virtual input devices
- Add input routing from clients
- Add platform abstraction
- **Test**: Moonlight input should work

#### Step 8: WebRTC Support (Low Risk)
- Add WebRTC manager
- Add browser-based streaming
- **Test**: Browser streaming should work

#### Step 9: Voice Input System (Low Risk)
- Add Whisper integration
- Add voice recognition
- Add voice API endpoints
- **Test**: Voice commands should work

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

### Phase 4: Testing Protocol
For each step:
1. Build new deb/binary
2. Deploy to helix container
3. Restart Hyprland process
4. Test VNC connection manually
5. Verify expected behavior (black->black->green->interactive)

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