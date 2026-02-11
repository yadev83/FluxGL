#!/usr/bin/env bash
set -e

# répertoire du script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$SCRIPT_DIR/.."
# dossier build
DEBUG_BUILD_DIR="$ROOT_DIR/build/Debug"
RELEASE_BUILD_DIR="$ROOT_DIR/build/Release"

# Remove the build directory
echo "🚮 Cleaning build artifacts..."
cmake --build "$DEBUG_BUILD_DIR" --target clean || true
cmake --build "$RELEASE_BUILD_DIR" --target clean || true
echo "✅ Clean complete!"