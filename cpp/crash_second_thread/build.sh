#!/bin/bash

set -e

if [ $# -ne 1 ]; then
    echo "Usage: $0 [fp|no-fp]"
    exit 1
fi

if [ "$1" = "fp" ]; then
    FP_FLAG="-fno-omit-frame-pointer"
elif [ "$1" = "no-fp" ]; then
    FP_FLAG="-fomit-frame-pointer"
else
    echo "Error: Argument must be either 'fp' or 'no-fp'"
    exit 1
fi

COMMAND="g++ main.cpp crash.cpp -O2 -g --std=c++20 ${FP_FLAG} "


${COMMAND} -fexceptions -o crash_thread_exceptions${1} 
${COMMAND} -fno-exceptions -o crash_thread_no_exceptions_${1} 
${COMMAND} -fno-exceptions -funwind-tables -o crash_thread_no_exceptions_unwind_tables_${1}
${COMMAND} -fno-exceptions -fasynchronous-unwind-tables -o crash_thread_no_exceptions_async_unwind_tables_${1}
${COMMAND} -fno-exceptions -funwind-tables -fasynchronous-unwind-tables -o crash_thread_no_exceptions_sync_and_async_unwind_tables_${1}