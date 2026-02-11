#!/usr/bin/env bash
set -e

# répertoire du script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$SCRIPT_DIR/.."
# dossier build
BUILD_DIR="$ROOT_DIR/build"

# Remove the build directory
echo "🚮 Cleaning build artifacts..."
cmake --build "$BUILD_DIR" --target clean || true
echo "✅ Clean complete!"