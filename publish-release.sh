#!/bin/bash
set -e

# HyprMoon Release Publisher
# Publishes deb packages as GitHub releases manually

REPO_OWNER="helixml"
REPO_NAME="hyprmoon"
RELEASE_DIR="/home/luke/pm/hyprmoon"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

log() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

# Check dependencies
check_deps() {
    log "Checking dependencies..."

    if ! command -v gh &> /dev/null; then
        error "GitHub CLI (gh) is not installed. Install with: sudo apt install gh"
        exit 1
    fi

    if ! gh auth status &> /dev/null; then
        error "GitHub CLI is not authenticated. Run: gh auth login"
        exit 1
    fi

    success "Dependencies check passed"
}

# Get version from changelog
get_version() {
    local changelog_file="$RELEASE_DIR/hyprland-0.41.2+ds/debian/changelog"

    if [[ ! -f "$changelog_file" ]]; then
        error "Changelog file not found: $changelog_file"
        exit 1
    fi

    # Extract version from first line: hyprmoon (0.41.2+ds-1.3+step8.1) unstable; urgency=medium
    local version=$(head -n1 "$changelog_file" | sed -n 's/hyprmoon (\([^)]*\)).*/\1/p')

    if [[ -z "$version" ]]; then
        error "Could not extract version from changelog"
        exit 1
    fi

    echo "$version"
}

# Find deb files for current version
find_deb_files() {
    local version="$1"
    local files=()

    # Look for main package
    local main_deb="$RELEASE_DIR/hyprmoon_${version}_amd64.deb"
    if [[ -f "$main_deb" ]]; then
        files+=("$main_deb")
    else
        error "Main deb package not found: $main_deb"
        exit 1
    fi

    # Look for backgrounds package
    local bg_deb="$RELEASE_DIR/hyprland-backgrounds_${version}_all.deb"
    if [[ -f "$bg_deb" ]]; then
        files+=("$bg_deb")
    else
        warn "Backgrounds deb package not found: $bg_deb"
    fi

    # Look for dev package
    local dev_deb="$RELEASE_DIR/hyprland-dev_${version}_amd64.deb"
    if [[ -f "$dev_deb" ]]; then
        files+=("$dev_deb")
    else
        warn "Dev deb package not found: $dev_deb"
    fi

    echo "${files[@]}"
}

# Generate release notes from changelog
generate_release_notes() {
    local version="$1"
    local changelog_file="$RELEASE_DIR/hyprland-0.41.2+ds/debian/changelog"

    # Extract release notes between version headers
    awk "
    /^hyprmoon \($version\)/ { found=1; next }
    found && /^hyprmoon \(/ { exit }
    found && /^  \*/ { gsub(/^  \* /, \"- \"); print }
    found && /^ --/ { exit }
    " "$changelog_file"
}

# Main release function
publish_release() {
    local version="$1"
    local tag="v$version"
    local deb_files=($2)

    log "Publishing release $tag..."

    # Check if release already exists
    if gh release view "$tag" --repo "$REPO_OWNER/$REPO_NAME" &> /dev/null; then
        warn "Release $tag already exists"
        read -p "Do you want to delete and recreate it? (y/N): " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            log "Deleting existing release..."
            gh release delete "$tag" --repo "$REPO_OWNER/$REPO_NAME" --yes
        else
            error "Aborting release publication"
            exit 1
        fi
    fi

    # Generate release notes
    local notes_file=$(mktemp)
    echo "# HyprMoon $version" > "$notes_file"
    echo "" >> "$notes_file"
    echo "## Changes" >> "$notes_file"
    generate_release_notes "$version" >> "$notes_file"
    echo "" >> "$notes_file"
    echo "## Installation" >> "$notes_file"
    echo "" >> "$notes_file"
    echo "Download the appropriate deb package for your system:" >> "$notes_file"
    echo "" >> "$notes_file"
    echo "- \`hyprmoon_${version}_amd64.deb\` - Main HyprMoon package" >> "$notes_file"
    if [[ " ${deb_files[@]} " =~ " hyprland-backgrounds " ]]; then
        echo "- \`hyprland-backgrounds_${version}_all.deb\` - Background wallpapers" >> "$notes_file"
    fi
    if [[ " ${deb_files[@]} " =~ " hyprland-dev " ]]; then
        echo "- \`hyprland-dev_${version}_amd64.deb\` - Development headers" >> "$notes_file"
    fi
    echo "" >> "$notes_file"
    echo "Install with: \`sudo dpkg -i hyprmoon_${version}_amd64.deb\`" >> "$notes_file"
    echo "" >> "$notes_file"
    echo "## System Requirements" >> "$notes_file"
    echo "" >> "$notes_file"
    echo "- Ubuntu 25.04 (Plucky Puffin) or compatible" >> "$notes_file"
    echo "- Wayland compositor support" >> "$notes_file"
    echo "- Graphics drivers with EGL/OpenGL ES support" >> "$notes_file"

    # Create release
    log "Creating GitHub release..."
    gh release create "$tag" \
        --repo "$REPO_OWNER/$REPO_NAME" \
        --title "HyprMoon $version" \
        --notes-file "$notes_file" \
        "${deb_files[@]}"

    rm -f "$notes_file"
    success "Release $tag published successfully!"

    # Show release URL
    local release_url="https://github.com/$REPO_OWNER/$REPO_NAME/releases/tag/$tag"
    log "Release URL: $release_url"
}

# Show file sizes
show_file_info() {
    local deb_files=($1)

    log "Files to be published:"
    for file in "${deb_files[@]}"; do
        if [[ -f "$file" ]]; then
            local size=$(du -h "$file" | cut -f1)
            local basename=$(basename "$file")
            echo "  - $basename ($size)"
        fi
    done
}

# Main script
main() {
    log "HyprMoon Release Publisher"
    log "========================="

    # Check dependencies
    check_deps

    # Get version
    local version=$(get_version)
    log "Current version: $version"

    # Find deb files
    local deb_files=($(find_deb_files "$version"))

    if [[ ${#deb_files[@]} -eq 0 ]]; then
        error "No deb files found for version $version"
        exit 1
    fi

    # Show file info
    show_file_info "${deb_files[@]}"

    # Confirm release
    echo
    warn "This will create a GitHub release for version $version"
    read -p "Do you want to continue? (y/N): " -n 1 -r
    echo

    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        log "Release publication cancelled"
        exit 0
    fi

    # Publish release
    publish_release "$version" "${deb_files[*]}"
}

# Run main function if script is executed directly
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi