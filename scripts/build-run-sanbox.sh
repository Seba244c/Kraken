#!/bin/bash
# Make sure it is run in the Kraken dir
current_dir="$(basename $PWD)"
if [[ "$current_dir" = "scripts" ]]; then
    cd ..
fi

cmake --build --preset build-debug-unix --target Sandbox
out/build/x64-debug-unix/Sandbox/Sandbox
