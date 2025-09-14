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

#### Step 1: Core Moonlight Protocol Infrastructure
- Add moonlight protocol files (.xml)
- Add basic protocol manager structure
- **Test**: VNC should still show black screen

#### Step 2: Window Manager Integration
- Add moonlight client tracking
- Add window creation for moonlight clients
- **Test**: VNC should still work, maybe see moonlight windows

#### Step 3: Green Screen Rendering
- Add green screen surface creation
- Add color fill functionality
- **Test**: VNC should show green screens for moonlight clients

#### Step 4: Input Handling
- Add moonlight input forwarding
- Add keyboard/mouse routing
- **Test**: VNC + moonlight clients should be interactive

#### Step 5: Optimization & Polish
- Add performance optimizations
- Add configuration options
- **Test**: Full functionality

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

## Enumerated HyprMoon Modifications

### From Previous Analysis:
1. **Protocol Files**:
   - moonlight-streaming-v1.xml (new protocol)
   - Protocol manager integration

2. **Core Changes**:
   - Window management for moonlight clients
   - Green screen surface rendering
   - Input routing and forwarding

3. **Rendering Changes**:
   - Custom surface creation
   - Color fill implementation
   - Compositor integration

4. **Configuration**:
   - Moonlight-specific settings
   - Protocol registration

### Risk Areas:
- **Renderer Changes**: Most likely to break VNC capture
- **Protocol Registration**: Could affect screencopy protocol
- **Window Management**: Might interfere with normal compositor flow

## Success Criteria
- VNC shows black screen with vanilla build
- VNC shows green screen for moonlight clients only
- Normal applications still render normally
- No crashes or stability issues
- Performance acceptable

## Implementation Notes
- Use Ubuntu's exact source as baseline
- Apply changes incrementally with git commits
- Test after each change
- Use build cache for fast iteration
- Document what breaks at each step