#!/bin/bash
# Make sure it is run in the Kraken dir
current_dir="$(basename $PWD)"
if [[ "$current_dir" = "scripts" ]]; then
    cd ..
fi

echo "Configure for Debug or Release mode?"
select dr in "Debug" "Release"; do
    case $dr in
        Debug ) cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 --preset x64-debug-unix; break;;
        Release ) cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 --preset x64-release-unix; break;;
    esac
done

# For clangd
if [ ! -f ./compile_commands.json ]; then
    ln out/build/x64-debug-unix/compile_commands.json .
fi
