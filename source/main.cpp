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

Shape* square;

// A Transform stores a position, rotation, and scale, and calculates the world matrix for you.
Transform2D transform;


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
	GLFWwindow* window = glfwCreateWindow(800, 600, "Transform Class", nullptr, nullptr);

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


	// Set Render mode.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);




	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear the screen.
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 0.0);



		// Draw geometry using the transform.
		transform.Rotate(.0002f);
		square->Draw(transform.GetMatrix());


		// Swap the backbuffer to the front.
		glfwSwapBuffers(window);

		// Poll input and window events.
		glfwPollEvents();

	}

	// Free memory from shape object
	delete square;

	// Free GLFW memory.
	glfwTerminate();


	// End of Program.
	return 0;
}
