#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [ "$1" = "stat" ]; then
    PERF_COMMAND="perf stat"
else
    PERF_COMMAND=""
fi

if [ -n "$1" ] && [ "$1" != "stat" ]; then
    echo "Usage: $0 [stat]"
    exit 1
fi

$PERF_COMMAND $SCRIPT_DIR/build-rel/compositor-benchmark --wayland-socket-name foo
