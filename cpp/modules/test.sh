#!/bin/bash

set -e

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
rm -rf build-gcc build-clang
cmake --preset=gcc && cmake --build build-gcc
cmake --preset=clang && cmake --build build-clang
