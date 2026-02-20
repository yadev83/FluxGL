#!/usr/bin/env bash
set -e

# build type
BUILD_TYPE=${1:-Debug}

# répertoire du script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$SCRIPT_DIR/.."
# dossier build
BUILD_DIR="$ROOT_DIR/build/$BUILD_TYPE"

echo "🚧 Configuring build..."

export VCPKG_DEFAULT_TRIPLET=x64-mingw-static
export VCPKG_DEFAULT_HOST_TRIPLET=x64-mingw-static

cmake -S . -G "MinGW Makefiles" -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DVCPKG_TARGET_TRIPLET=x64-mingw-static \
    -DCMAKE_TOOLCHAIN_FILE="$ROOT_DIR"/vendor/vcpkg/scripts/buildsystems/vcpkg.cmake
echo "✅ Configuration complete!"

echo "🔨 Building FluxGL & Examples..."
cmake --build "$BUILD_DIR"
echo "✅ Build complete!"