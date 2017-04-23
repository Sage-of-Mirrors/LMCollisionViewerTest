#include "stdafx.h"
#include "Collision.h"

Collision::Collision(std::string in_File) {
	m_isRendered = true;
	m_vertBufObj = 0;
	m_vertAtrObj = 0;

	std::string copy = in_File;
	std::string::size_type idx;
	idx = copy.rfind('.');

	// There was an extension on the file
	if (idx != std::string::npos)
	{
		std::string extension = copy.substr(idx + 1);

		// OBJ found! Loading face data and giving the faces default properties
		if (extension == "obj" || extension == "OBJ")
		{
			m_LoadObj(in_File);
		}
		// Compiled collision found! Loading the mp and JMP files.
		else if (extension == "mp" || extension == "MP")
		{
			m_LoadCompiled(in_File);
		}
		// JSON found! Loading face data and their properties
		else if (extension == "json" || extension == "JSON")
		{
			m_LoadJson(in_File);
		}
		// Extension not recognized!
		else
		{

		}
	}
	// No extension found!
	else
	{

	}

	for (std::vector<Face>::iterator it = m_Faces.begin(); it != m_Faces.end(); it++)
	{
		it->CreateBufferObject(m_VertexBuffer, m_attributeBuffer);
	}

	glGenBuffers(1, &m_vertBufObj);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertBufObj);
	glBufferData(GL_ARRAY_BUFFER, m_VertexBuffer.size() * sizeof(sf::Vector3f), &m_VertexBuffer[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_vertAtrObj);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertAtrObj);
	glBufferData(GL_ARRAY_BUFFER, m_attributeBuffer.size() * sizeof(int), &m_attributeBuffer[0], GL_STATIC_DRAW);
}

void Collision::SaveJSON(std::string out_file) {
	Json::Value collision;
	Json::Value triVec(Json::arrayValue);

	for (std::vector<Face>::iterator it = m_Faces.begin(); it != m_Faces.end(); it++) {
		Json::Value tri;
		it->JSONSerialize(tri);
		triVec.append(tri);
	}

	collision["collision"]["triangles"] = triVec;

	std::ofstream stm(out_file);
	stm << collision;
	stm.close();
}

void Collision::SaveCompiled(std::string out_file_dir) {
}

void Collision::m_LoadObj(std::string objPath) {
	std::vector<sf::Vector3f> vertices;
	std::vector<sf::Vector3f> normals;

	std::ifstream objInput(objPath);
	std::string line;

	while (getline(objInput, line))
	{
		std::string lineType = line.substr(0, line.find(' '));
		line.erase(0, line.find(' ') + 1);

		// Vertex or normal
		if (lineType == "v" || lineType == "vn")
		{
			std::string xComp = line.substr(0, line.find(' '));
			line.erase(0, line.find(' ') + 1);
			std::string yComp = line.substr(0, line.find(' '));
			line.erase(0, line.find(' ') + 1);
			std::string zComp = line;

			sf::Vector3f newVec(stof(xComp), stof(yComp), stof(zComp));

			if (lineType == "v")
				vertices.push_back(newVec);
			else
				normals.push_back(newVec);
		}
		//Face
		else if (lineType == "f")
		{
			Face face;
			for (int i = 0; i < 3; i++)
			{
				std::string vert = line.substr(0, line.find(' '));
				std::string vertPos = vert.substr(0, vert.find('/'));
				vert.erase(0, vert.find('/') + 1);
				std::string vertUV = vert.substr(0, vert.find('/'));
				vert.erase(0, vert.find('/') + 1);
				std::string vertNrm = vert;

				int vertPosIndex = std::stoi(vertPos) - 1;
				int vertNrmIndex = std::stoi(vertNrm) - 1;
				face.SetVertexPosition(vertices[vertPosIndex], i);
				face.SetVertexNormal(normals[vertNrmIndex], i);

				line.erase(0, line.find(' ') + 1);
			}

			face.CalculateFaceNormal();
			m_Faces.push_back(face);
		}
		// Unsupported token (#, o, s)
		else
		{
			continue;
		}
	}

	objInput.close();
}

void Collision::m_LoadCompiled(std::string compiledRootPath) {

}

void Collision::m_LoadJson(std::string jsonPath) {
	std::ifstream in(jsonPath);
	Json::Value colRoot;
	in >> colRoot;
	in.close();

	for (int i = 0; i < colRoot["collision"]["triangles"].size(); i++) {
		Face face;
		face.JSONDeserialize(colRoot["collision"]["triangles"][i]);
		m_Faces.push_back(face);
	}
}

void Collision::SetRendered(bool rendered) {
	m_isRendered = rendered;
}

std::vector<Face>& Collision::GetFaces() {
	return m_Faces;
}

void Collision::Render()
{
	if (!m_isRendered)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, m_vertBufObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertAtrObj);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(sf::Vector3f), 0);

	glDrawElements(GL_TRIANGLES, m_attributeBuffer.size(), GL_INT, 0);

	glDisableVertexAttribArray(0);
}