#!/bin/bash

set -e

# Create a build directory if it doesn't exist
mkdir -p build

# Compile Swift files without Metal frameworks
swiftc -o build/BasicWindow main.swift \
    -framework Cocoa

# Make the binary executable
chmod +x build/BasicWindow

# Run the application if requested
if [ "$1" == "run" ]; then
    ./build/BasicWindow
fi

echo "Build completed successfully!"
