#include "stdafx.h"
#include "Face.h"

Face::Face() {
	m_ColProperties.polyMaterial = 0;
	m_ColProperties.isLadder = false;
	m_ColProperties.ignorePointer = false;

	m_SoundProperties.soundMaterial = 0;
	m_SoundProperties.soundEchoSwitch = 0;

	m_FaceNormal = sf::Vector3f(0.0f, 0.0f, 0.0f);
	m_unknownFloat = 50.0f;
}

void Face::SetVertexPosition(sf::Vector3f vec, int index) {
	m_VertexPositions[index] = vec;
}

void Face::SetVertexNormal(sf::Vector3f vec, int index) {
	m_VertexNormals[index] = vec;
}

void Face::SetPolyMaterial(int mat) {
	m_ColProperties.polyMaterial = mat;
}

void Face::SetIsLadder(bool is) {
	m_ColProperties.isLadder = is;
}

void Face::SetIgnorePointer(bool ignore) {
	m_ColProperties.ignorePointer = ignore;
}

void Face::SetSoundMaterial(int mat) {
	m_SoundProperties.soundMaterial = mat;
}

void Face::SetEchoSwitch(int swit) {
	m_SoundProperties.soundEchoSwitch = swit;
}

void Face::CalculateFaceNormal() {
	sf::Vector3f p0 = m_VertexPositions[1] - m_VertexPositions[0];
	sf::Vector3f p1 = m_VertexPositions[2] - m_VertexPositions[0];

	// Cross product
	// i     j     k
	// x0    y0    z0
	// x1    y1    z1
	//
	// Then calculate determinants:
	// X:
	// y0     z0
	// y1     z1
	//
	// Y (Take negative of the determinant):
	// x0     z0
	// x1     z1
	//
	// Z:
	// x0     y0
	// x1     y1
	float x = (p0.y * p1.z) - (p0.z * p1.y);
	float y = -((p0.x * p1.z) - (p0.z * p1.x));
	float z = (p0.x * p1.y) - (p0.y * p1.x);
	float normalization = sqrtf((x*x) + (y*y) + (z*z));

	m_FaceNormal = sf::Vector3f(x / normalization, y / normalization, z / normalization);
}

void Face::CreateBufferObject(std::vector<sf::Vector3f>& vectorBuffer, std::vector<int>& indexBuffer) {
	for (int i = 0; i < 3; i++)
	{
		vectorBuffer.push_back(m_VertexPositions[i]);
		indexBuffer.push_back(vectorBuffer.size() - 1);
	}
}

CollisionProperties& Face::GetCollisionProperties() {
	return m_ColProperties;
}

SoundProperties& Face::GetSoundProperties() {
	return m_SoundProperties;
}