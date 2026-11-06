#!/bin/sh
# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Techniix (Ship of Harkinian Haiku port)
#
# Build Ship of Harkinian on Haiku (x86_64).
#
#   scripts/haiku/build.sh            configure + build into build-haiku/
#   scripts/haiku/build.sh --deps     install build dependencies with pkgman first
#   scripts/haiku/build.sh --package  also make a zip with cpack
#
# Environment:
#   BUILD_TYPE   Release (default), Debug, RelWithDebInfo
#   BUILD_DIR    build directory (default: build-haiku)
#   JOBS         parallel jobs (default: CPU count)

set -e

cd "$(dirname "$0")/../.."

if [ "$(uname -s)" != "Haiku" ]; then
	echo "This script is meant to run on Haiku." >&2
	exit 1
fi

if [ "$(getarch 2>/dev/null || uname -m)" != "x86_64" ]; then
	echo "Only x86_64 Haiku is supported (32-bit x86 lacks a new enough compiler for this codebase)." >&2
	exit 1
fi

BUILD_TYPE="${BUILD_TYPE:-Release}"
BUILD_DIR="${BUILD_DIR:-build-haiku}"
JOBS="${JOBS:-$(nproc 2>/dev/null || echo 2)}"

INSTALL_DEPS=0
PACKAGE=0
for arg in "$@"; do
	case "$arg" in
		--deps) INSTALL_DEPS=1 ;;
		--package) PACKAGE=1 ;;
		*) echo "Unknown option: $arg" >&2; exit 1 ;;
	esac
done

if [ "$INSTALL_DEPS" = 1 ]; then
	# One at a time so a renamed package doesn't block the rest.
	missing=""
	for pkg in $(grep -v '^#' scripts/haiku/pkgman.txt); do
		echo "==> pkgman install $pkg"
		pkgman install -y "$pkg" || missing="$missing $pkg"
	done
	if [ -n "$missing" ]; then
		echo "" >&2
		echo "Could not install:$missing" >&2
		echo "Search for the current names with: pkgman search <name>" >&2
	fi
fi

git submodule update --init

cmake -S . -B "$BUILD_DIR" -G Ninja -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
cmake --build "$BUILD_DIR" --target GenerateSohOtr -j "$JOBS"
cmake --build "$BUILD_DIR" -j "$JOBS"

if [ "$PACKAGE" = 1 ]; then
	(cd "$BUILD_DIR" && cpack)
	echo "Package written to _packages/"
fi

echo ""
echo "Done. Run it with:  $BUILD_DIR/soh/soh"
echo "Put your Ocarina of Time ROM (.z64/.n64/.v64) in $BUILD_DIR/soh/ or pick it when asked."
