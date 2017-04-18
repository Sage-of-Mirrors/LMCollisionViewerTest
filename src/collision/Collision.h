#pragma once
#include "bstream.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Face.h"

class Collision {
	bool isRendered;
	std::vector<Face> m_Faces; // Holds face data, which includes position and properties.
	void m_LoadObj(std::string); // Loads faces from an OBJ file with default properties.
	void m_LoadJson(std::string); // Loads faces and their properties from a JSON file.
	void m_LoadCompiled(std::string); // Loads faces and their properties from the MP (for faces) and JMP (for properties) files.
public:
	Collision(std::string);
	void SetRendered(bool);
	void Render();
	std::vector<Face>& GetFaces(); // Returns a reference to the vector holding the face data
};