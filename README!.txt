Documentation Author: Niko Procopi 2019

This tutorial was designed for Visual Studio 2017 / 2019
If the solution does not compile, retarget the solution
to a different version of the Windows SDK. If you do not
have any version of the Windows SDK, it can be installed
from the Visual Studio Installer Tool

Welcome to the Shaders Tutorial!
Prerequesites: World Matrices

[ If you are not interested in definitions ]
[ and just want to see how the code works, ]
[ scroll down to the bottom of the page    ]

Shaders are small programs that are executed on the GPU.
Prior to using OpenGL, students may have used C++ to
write programs that use the CPU, and these programs may have
used hundreds or thousands of lines of C++. Shaders on the GPU
are small programs, that may only take 10-50 lines. In previous
tutorials, GLSL programs have been sent to the GPU that have been
pre-written by the authors of the tutorials. Now, we will show
you how to write your own shaders.

There are two types of shaders that we will focus on for now:
Vertex Shaders and Fragment Shaders.

Vertex shaders handle the processing of each vertex:
where the vertex is, what color it is, etc. This may sound
simple, but think about animations in games, think about
movement, rotation, and scaling of objects, that all happens
in the vertex shader.

After all the vertices in the scene are processed, they go
to the Rasterizer. The Rasterizer turns the vertices into
a grid of pixels. When the rasterizer, every pixel on the 
screen is either black (if there is no geometry on that pixel),
or white (if there is geometry at that pixel). We do not need 
to program the rasterizer, nobody does, it has fixed functions 
inside the GPU driver. We have the ability to toggle options 
to decide how the Rasterizer does its job, but we dont need to 
write shaders for it.

The fragment shader takes the pixels on the screen, and applies
color to them: textures, lights, shadows, reflections, and 
everything color-related happens here

[ How the Vertex Shader works ]

Shaders use the language GLSL, they run on the GPU while
the C++ code runs on the CPU

First we set the version of GLSL
	#version 430
	
Next, remember all the times we set "attributes" in C++?
Now we finally see why those are important. For the GPU
to process each part of the vertex (position and color),
we have to tell it how to divide the vertex into its sub-parts
(position and color)
	layout (location=0) in vec3 position;
	layout (location=1) in vec3 color;
	
We need to set an output for the color, to give color
to the rasterizer. We need to do this so that color
is interpolated between the vertices for each pixel 
	out vec3 outColor;
	
We don't need to do this for position because there
is already a place for it by default, we will use 
gl_Position for that
	
We then have "void main" which is a function, just like
functions in C++. Inside this main function, for this simple
tutorial, we won't be changing the position of the vertices,
or the color, we will just take them as input, and then pass them
to the rasterizer
	gl_Position = vec4(position, 1.0);
	outColor = color;
	
They are given to the rasterizer as soon as main() ends,
and then the rasterizer gives them to the fragment shader.
In the future, we will be altering data in the vertex shader
for animation: movement, scaling, rotating

[ How the Fragment Shader works ]

Again, first we set the version
	#version 430
	
We take color as input from the rasterizer,
we dont need to do this with position because 
that happens automatically
	in vec3 outColor;
	
We need to make another output for color,
this output goes to the screen, and again, we
do not need to modify it in this tutorial
	color = vec4(outColor, 1.0f);
	
The color in the fragment shader is NOT the same
as the color in the vertex shader. Changing colors
in the vertex shader can only change what the color
is at each vertex of a triangle, changing colors in 
the fragment shader can allow us to set the color of
any given pixel, which is used for textures, lights,
shadows, etc.

[ How to compile and run GLSL code ]

In C++, we compile our code in Visual Studio, we get an EXE file,
and then we run the file.

Inside our C++ program, while it is running, we need to load our
GLSL code, compile it, and then run it on the GPU while our C++
is running on the CPU.

The code in shader.cpp will handle this, and it will probably
never need to be changed. If you are interested in how
it works, the comments expalain every step:
-Load the code for each shader
-Compile each shader
-Add each shader together to make a GPU program
-Bind the shader in your draw function
-Call a draw command, which uses the shader to draw