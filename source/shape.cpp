/*
Title: World Matrices
File Name: shape.cpp
Copyright ? 2016
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

#include "../header/shape.h"

Shape::Shape(std::vector<glm::vec2> vertices, std::vector<unsigned int> indices)
{
	m_vertices = vertices;
	m_indices = indices;
	// Create the shape by setting up buffers

	// Set up vertex buffer
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	// NOTE: in this example we use GL_DYNAMIC_DRAW instead of GL_STATIC_DRAW, because we will be changing vertices on the fly!
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Set up index buffer same as above
	glGenBuffers(1, &m_indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_indexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Shape::~Shape()
{
	// Clear buffers for the shape object when done using them.
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
}



void Shape::Draw(glm::mat3 worldMatrix)
{
	// Before we draw, we need to transform our shape info using the matrix
	std::vector<glm::vec2> tempVertexData;

	// Loop through each vertex
	for (int i = 0; i < m_vertices.size(); i++)
	{
		//
		glm::vec3 v = glm::vec3(m_vertices[i], 1);
		v = worldMatrix * v;
		tempVertexData.push_back((glm::vec2)v);
	}




	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);


	// Here we update our vertex buffer with our newly calculated vertex positions!
	// glBufferSubData can be used to edit values within a buffer.
	// It works very similarly to std::memcpy, if you are familiar with that function. If not, you should check it out!
	// We need:
	// Buffer binding location
	// Offset into the buffer to start writing
	// Size of data to write (bytes)
	// Pointer to start location of the data we want to copy from
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(glm::vec2), &tempVertexData[0]);



	// Set Vertex Attribute at index 0
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Bind index buffer to GL_ELEMENT_ARRAY_BUFFER, and enable vertex attribute
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	glEnableVertexAttribArray(0);

	// Draw all indices in the index buffer
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0);

	// Disable vertex attribute and unbind index buffer.
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
