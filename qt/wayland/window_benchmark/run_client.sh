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

# Commented out since they have similar performance.
# only decos cause it to be slower

# echo "Testing xdg-shell..."
# WAYLAND_DISPLAY=foo QT_WAYLAND_SHELL_INTEGRATION=xdg-shell $SCRIPT_DIR/build-dev/client -n 3

# echo "Testing xdg-shell no decos..."
# QT_WAYLAND_DISABLE_WINDOWDECORATION=1 WAYLAND_DISPLAY=foo QT_WAYLAND_SHELL_INTEGRATION=xdg-shell $SCRIPT_DIR/build-dev/client -n 3

# echo "Testing qt-shell..."
# WAYLAND_DISPLAY=foo QT_WAYLAND_SHELL_INTEGRATION=qt-shell $SCRIPT_DIR/build-dev/client -n 3

#echo "Testing wl-shell..."
#WAYLAND_DISPLAY=foo QT_WAYLAND_SHELL_INTEGRATION=wl-shell $SCRIPT_DIR/build-dev/client -n 3

echo "Testing wl-shell no decos..."
QT_WAYLAND_DISABLE_WINDOWDECORATION=1 WAYLAND_DISPLAY=foo QT_WAYLAND_SHELL_INTEGRATION=wl-shell $PERF_COMMAND  $SCRIPT_DIR/build-rel/client -n 5

pkill compositor-be
