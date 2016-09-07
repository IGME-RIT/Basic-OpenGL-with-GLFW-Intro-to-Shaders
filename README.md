# Basic OpenGL with GLFW: Intro to Shaders

This program introduces basic concepts involved in writing an OpenGL program.

# About

Shaders are the programs that run on you graphics card.
Before this tutorial, everything was done with default shaders. Now, we'll go through the process of compiling and using a shader in an OpenGL program.
This example builds on top of the World Matrix example, and abstracts the world matrix math into a transform2d class.

# Setup

You will need to have CMake installed on your computer, and properly added to your path.
In order to setup, run the following in a shell, then open the project in your preferred editor.
Windows setup has been configured for use with Visual Studio.

Windows:
```
cd path/to/folder
setup.cmd
```
Linux:
```
cd path/to/folder
./setup
```
