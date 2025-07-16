#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

PERF_COMMAND=""
NO_SURFACE=""
FULLSCREEN=""

for arg in "$@"; do
    if [ "$arg" = "stat" ]; then
        PERF_COMMAND="perf stat"
    elif [ "$arg" = "noSurface" ]; then
        NO_SURFACE="--no-surface"
    elif [ "$arg" = "--fullscreen" ]; then
        FULLSCREEN="--fullscreen"
    else
        echo "Usage: $0 [stat] [noSurface] [--fullscreen]"
        exit 1
    fi
done

$PERF_COMMAND $SCRIPT_DIR/build-rel/compositor-benchmark --wayland-socket-name foo $NO_SURFACE $FULLSCREEN
