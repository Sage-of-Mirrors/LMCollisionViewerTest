#pragma once
#include "Face.h"
#include <GL\glew.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "json\json.h"

class Collision {
	bool m_isRendered; // Determines if the collision mesh should be rendered.
	GLuint m_vertBufObj; // The ID of the vertex buffer object (vertex data) for the collision mesh
	GLuint m_vertAtrObj; // The ID of the vertex attribute object (vertex index data) for the collision mesh
	std::vector<sf::Vector3f> m_VertexBuffer; // Holds vertex data for rendering.
	std::vector<int> m_attributeBuffer; // Holds vertex attribute data (vertex indices for triangles) for rendering.
	std::vector<Face> m_Faces; // Holds face data, which includes position and properties.
	void m_LoadObj(std::string); // Loads faces from an OBJ file with default properties.
	void m_LoadJson(std::string); // Loads faces and their properties from a JSON file.
	void m_LoadCompiled(std::string); // Loads faces and their properties from the MP (for faces) and JMP (for properties) files.
public:
	Collision(std::string); // Creates a Collision instance from the specified file or file path.
	void SaveJSON(std::string); // Saves collision data to disk as a JSON file.
	void SaveCompiled(std::string); // Saves collision data to disk as compiled game data.
	void SetRendered(bool); // Sets whether the collision mesh is rendered in the viewport.
	void Render(); // Renders the collision mesh to the viewport.
	std::vector<Face>& GetFaces(); // Returns a reference to the vector holding the face data
};