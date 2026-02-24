# FluxGL

A barebones graphics engine made with C++, OpenGL and GLFW.

## Installation

### Setup Compilation Environment
First you need to make sure that your Windows environment is C/C++ ready.
The best way to do this if you are just starting out is to grab the latest winlibs build from [their official website](https://winlibs.com/). This will provide you with a subset of development tools built for Windows. It includes for example : CMake, make, and MinGW64 compilers (gcc/g++).  
Once you have the latest version of winlibs installed and with the `mingw64/bin` folder setup in your path, you should be good to go. You can always run `where gcc` or `where g++` to make sure that the compilers are in your path and adjust your environment accordingly if needed.

Make sure to use the Git Bash (or similar) on Windows, to make `.sh` files executable.

### Dependencies

Clone this repository with :

```bash
git clone https://github.com/yadev83/FluxGL.git
``` 

Then pull the necessary git submodules with :
```bash
git submodule update --init --recursive
``` 

Later on, you can update the submodules with : 
```bash
git submodule update --recursive
```

Submodules are located in the `vendor` folder. The following dependencies are used :
- [VCPKG](https://github.com/microsoft/vcpkg) : The package manager for C++

### Build

There are several build/configuration scripts available to you in the `scripts` folder :  
- `build.sh`: Configure the CMake project and build the library and examples while also installing vcpkg listed dependencies.
- `clean.sh`: Cleans the build artifacts.  

If you are using Visual Studio Code, you can use the `tasks.json` commands to run the scripts in a more integrated way.
