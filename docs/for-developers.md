# Building and Running
Currently building is tested to work on MacOS and Windows

## Windows
Install [Ninja](https://ninja-build.org/)

After cloning this repository make sure that the submodules are cloned by running `git submodule update --init --recursive --remote` from the base dir.

Using windows it is recomended to have installed Visual Studios C++ tools or use Visual Studio to run & build this project. This should automatically install `vcpkg` and the MSVC compiler.

Using **Visual Studio**, simply open the base folder as a Visual Studio project and let VS automatically configure the project. At the top you can then switch between the Debug and Build profile. If `vcpkg` is not found or `VCPKG_ROOT` is not set, please make sure the developers options for C++ is installed with VS.

Using **CLion** install the MSVC compiler using Visual Studio Installer and install `vcpkg` manualy, using this [guide from Microsoft](https://learn.microsoft.com/da-dk/vcpkg/get_started/get-started?pivots=shell-cmd). Make sure to set the enviroment variable `VCPKG_ROOT` and update the `PATH` globally.
Now you should be able to open the base folder using **CLion**. When using the IDE use one of the CMake presets provided "x64-debug/release-win".

I recommend using an IDE, personally i use CLion. Using VS it is recomended to have an extension like VisualTomato or ReSharper installed.


## MacOS / Linux?
*Ha this barely works on my machine*

* First make sure to install git, g++, gcc and ninja.
* Clone this project and run `scripts/fix-depedencies` to make ensure that submodules are cloned
* Then install `vcpkg` manualy, using this [guide from Microsoft](https://learn.microsoft.com/da-dk/vcpkg/get_started/get-started?pivots=shell-cmd).
 * Make sure to set the enviroment variable `VCPKG_ROOT` and update the `PATH` globally.
* icu (sub depedency) currently requires the following programs from the system package: autoconf automake autoconf-archive
 * For Mac using Homebrew
  * `brew install autoconf autoconf-archive automake`
 * On Debian and Ubuntu derivatives
  * `sudo apt-get install autoconf automake autoconf-archive`
 * Using other linux package managers
  * `sudo dnf install autoconf automake autoconf-archive`
  * `sudo pacman -S autoconf automake autoconf-archive`
  * `apk add autoconf automake autoconf-archive`
* Other depedencies might also have depedencies, please see compiler log. I think glfw3 will ask for `xinerama, xcursor, xorg, libglu1-mesa, pkg-config`

Then either use **CLion**, you just have to open the base folder as a project and the IDE should recognize the CMake files. Then change the CMake profile to the provided presets "x64-debug/release-unix"

Or compile it from the commandline. In the base directory run `scripts/cmake-configure.sh` and choose the desired build configuration. Cmake should then pull all the depedencies

# Please help
This guide was written in one go whilst still building this for my Mac. Please contact me if you have any issues and make a PR with any improvments to his guide
