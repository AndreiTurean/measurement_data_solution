# Prerequisites for building this project
## VCPKG
*	VCPKG is used package management. It helps us manage our dependecies in order to create a cross platform and cross compile project.
*	VCPKG reduce the need of having already installed packages, for example google test, and get them if you require them.

##	CMake
*	CMake will generate the build system and will help us develop cross platform applications.

##	Git
##	WS:
*	WSL will be used for cross compiling

# How to build the project

After all the prequisites are fullfilled, then you can proceed to select one CMake preset, keep in mind that you need to get one that will suite your machine, and run the CMake command using the preset. 
See example of how to see all the availabe presets:
![Preset Lists](./img/preset_list.png)

Then choose one preset that fitst your machine and run it:
![Preset run](./img/run_preset.png)

## Integration with Visual Studio Code

In order to integrate the build of the framework with visual studio code, you need first of all install the CMake package from the package exteensions.

## Integration with Visual Studio

Assuming that you have a modern Visual Studio, it should already be integrated. What you will need to do more is to configure your WSL in order to run the code using the Ubuntu Kernel.