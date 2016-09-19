/*
Title: World Matrices
File Name: Main.cpp
Copyright � 2016
Author: David Erbelding
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include "../../header/shape.h"
#include "../header/transform2d.h"
#include <iostream>

Shape* square;

// A Transform stores a position, rotation, and scale, and calculates the world matrix for you.
Transform2D transform;

// Here we store the GL indices to our shader objects.
GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;

// Window resize callback
void resizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char **argv)
{
	// Initializes the GLFW library
	glfwInit();

	// Initialize window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Basic Shaders", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	//set resize callback
	glfwSetFramebufferSizeCallback(window, resizeCallback);

	// Initializes the glew library
	glewInit();


	// Indices for square (-1, -1)[2] to (1, 1)[1]
	// [0]------[1]
	//	|		 |
	//	|		 |
	//	|		 |
	// [2]------[3]

	// Create square vertex data.
	std::vector<glm::vec2> vertices;
	vertices.push_back(glm::vec2(-1, 1));
	vertices.push_back(glm::vec2(1, 1));
	vertices.push_back(glm::vec2(-1, -1));
	vertices.push_back(glm::vec2(1, -1));

	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(1);


	// Create shape object
	square = new Shape(vertices, indices);
	

	transform.SetScale(.25f);
	transform.SetPosition(glm::vec2(.25, .25));





	// SHADER STUFF

	// OpenGL compiles shaders from code at runtime, so we'll be using strings to create them.
	// It's better to load shaders from a glsl file, but for this example, the strings will be hard coded.


	// The vertex shader operates once on every vertex sent to the shader, and outputs a modified vertex.
	// This vertex shader is just going to flip everything upside-down.
	std::string vertShaderText =

		// This line must be at the top of every shader. It tells the compiler what spec we are using.
		"#version 400 core \n"

		// This is our Vertex Attribute (see the draw method in shape.cpp)
		"layout(location = 0) in vec2 in_position;"

		// The main function is where the actual program takes place.
		"void main(void)"
		"{"
			// Here we take the position passed in, and flip the y coordinate.
			"vec2 flippedPosition = vec2(in_position.x, -in_position.y);"
			
			// Vertex shaders always output a vec4, so we have to make one using the vec2 we have.
			// gl_Position is the output of the vertex shader.
			"gl_Position = vec4(flippedPosition, 0, 1);"
		"}";




	// The fragment/pixel shader operates once on every pixel that is rendered to the screen.
	// This is the step that actually creates our image.
	std::string fragShaderText =

		// Version declaration
		"#version 400 core \n"

		"void main(void)"
		"{"
			// For the demo, we're just going to set the color to red.
			// gl_FragColor is our output to the backbuffer
			"gl_FragColor = vec4(1, 0, 0, 1);" 
		"}";





	// glCreateShader creates a vertex shader and returns its index.
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Unfortunately for us, OpenGL doesn't use standard library strings, which means we need:
	const char* shaderCodePointer = vertShaderText.data();	// A pointer to the first character ()
	int shaderCodeLength = vertShaderText.size();			// The length of that string.

	// Here OpenGL copies the shader code into the shader at the index we gave it.
	glShaderSource(vertexShader, 1, &shaderCodePointer, &shaderCodeLength);
	// It's actually taking an array of pointers to characters, and an array of pointers to lengths, where
	// (in our case) 1 is the array length. If there were more than one, OpenGL would concatenate them.



	// Now that the shader code is stored, we compile the code for the gpu.
	glCompileShader(vertexShader);


	// Here we check to make sure the shader compiled correctly.
	// Note: This is technically not required, but it is considered good practice, and is extremely helpful when debugging.
	// OpenGL will otherwise fail silently if your shader does not compile, and it's very difficult to figure out why.

	GLint isCompiled;
	// This line is asking OpenGL what the compile status of the vertex shader is.
	// It writes the value to isCompiled, which we pass in by reference.
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);

	// If it doesn't compile 
	if (!isCompiled)
	{
		// We make space for an error message.
		char infolog[1024];
		// We ask openGL to write its error message to the space we gave it.
		glGetShaderInfoLog(vertexShader, 1024, NULL, infolog);
		// Print the error.
		std::cout << "Vertex Shader compile failed with error: " << std::endl << infolog << std::endl;

		// It's good to delete the shader if it doesn't compile correctly, this way we don't leak memory.
		glDeleteShader(vertexShader);
	}











	// Now we do the exact same thing for the fragment shader...
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Get the char* and length
	shaderCodePointer = fragShaderText.data();
	shaderCodeLength = fragShaderText.size();

	// Set the source code and compile.
	glShaderSource(fragmentShader, 1, &shaderCodePointer, &shaderCodeLength);
	glCompileShader(fragmentShader);


	// Check if the fragmentShader compiles:
	// If it failed, print an error and delete it.
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (!isCompiled)
	{
		char infolog[1024];
		glGetShaderInfoLog(fragmentShader, 1024, NULL, infolog);
		std::cout << "Fragment Shader compile failed with error: " << std::endl << infolog << std::endl;
		glDeleteShader(fragmentShader);
	}





	// Once the shaders are loaded, we can create a Shader Program.
	// These can contain a collection of shaders for different steps, and combine them into one step.
	shaderProgram = glCreateProgram();
	
	// Attack the vertex and fragment shaders to our program.
	// It's possible to attach more than one of the same shader stage, however
	// only one of them can have a main function, because they will essentially be combined into one program.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Tells gl to set up the connections between the shaders we have attached.
	// After this we should be ready to roll.
	glLinkProgram(shaderProgram);






	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
        // Calculate delta time.
        float dt = glfwGetTime();
        // Reset the timer.
        glfwSetTime(0);

		// Clear the screen.
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 0.0);


		// rotate square
		transform.Rotate(dt);

		// Here we set which shader program to render with.
		///Commenting this line out will revert to drawing with default shaders.
		///(which seem to just pass the values you set through without doing anything)
		glUseProgram(shaderProgram);

		square->Draw(transform.GetMatrix());

		// Stop using the shader program.
		glUseProgram(0);

		// Swap the backbuffer to the front.
		glfwSwapBuffers(window);

		// Poll input and window events.
		glfwPollEvents();

	}

	// Free memory from shader program and individual shaders
	glDeleteProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// Free memory from shape object
	delete square;

	// Free GLFW memory.
	glfwTerminate();


	// End of Program.
	return 0;
}
