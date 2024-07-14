#!/bin/bash
# Make sure it is run in the Kraken dir
current_dir="$(basename $PWD)"
if [[ "$current_dir" = "scripts" ]]; then
    cd ..
fi

echo Recursively pulling submodules...
git submodule update --init --recursive --remote

echo Ensuring ninja is installed...
if ! command -v ninja &> /dev/null
then
    echo "Please install ninja"
    exit 1
fi

echo Ensuring autoconf is installed...
if ! command -v autoconf &> /dev/null
then
    echo "sudo apt-get install autoconf"
    echo "brew install autoconf"
    echo "brew install autoconf-archive"
    exit 1
fi

echo Ensuring autoconf is installed...
if ! command -v automake &> /dev/null
then
    echo "sudo apt-get install autotools-dev"
    echo "sudo apt-get install automake"
    echo "brew install automake"
    exit 1
fi

echo Ensuring vcpkg is installed...
if ! command -v vcpkg &> /dev/null
then
    echo "vcpkg could not be found!!"
    echo "vcpkg is Microsoft's package manager for C++, and is what allows for me to maintain Kraken"
    echo "1. To install vcpkg, in another directory clone vcpkg from github"
    echo "  git clone https://github.com/microsoft/vcpkg.git"
    echo "2. Navigate to the vcpkg directory and run the boostrap script"
    echo "  Windows: cd vcpkg && bootstrap-vcpkg.bat"
    echo "  Linux/Mac: cd vcpkg && ./bootstrap-vcpkg.sh"
    echo "3. Add VCPKG_ROOT enviroment variable and update PATH"
    echo "  This varies depeding on system"
    echo "  VCPKG_ROOT should be the path to the vcpkg directory and PATH should include VCPKG_ROOT"
    echo "Optional 4. Add terminal completions"
    echo "  See https://learn.microsoft.com/da-dk/vcpkg/commands/integrate#vcpkg-autocompletion"
    exit 1
fi
