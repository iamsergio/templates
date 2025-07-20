#!/bin/bash

set -e

# Create a build directory if it doesn't exist
mkdir -p build

# Compile Swift files
swiftc -o build/MetalTriangle main.swift Renderer.swift \
    -framework Metal \
    -framework MetalKit \
    -framework Cocoa

# Make the binary executable
chmod +x build/MetalTriangle

# Run the application if requested
if [ "$1" == "run" ]; then
    ./build/MetalTriangle
fi

echo "Build completed successfully!"
