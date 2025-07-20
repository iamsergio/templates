#!/bin/bash

set -e

# Create a build directory if it doesn't exist
mkdir -p build

# Compile SimpleTriangle.swift
swiftc -o build/SimpleTriangle SimpleTriangle.swift \
    -framework Metal \
    -framework MetalKit \
    -framework Cocoa

# Make the binary executable
chmod +x build/SimpleTriangle

# Run the application if requested
if [ "$1" == "run" ]; then
    ./build/SimpleTriangle
fi

echo "Build completed successfully!"
