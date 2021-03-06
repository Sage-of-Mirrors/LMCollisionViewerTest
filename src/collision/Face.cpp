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

void Face::JSONSerialize(Json::Value& root) {

	for (int i = 0; i < 3; i++) {
		std::string iAsString = std::to_string(i);
		root["vtxPos" + iAsString]["x"] = Json::Value(m_VertexPositions[i].x);
		root["vtxPos" + iAsString]["y"] = Json::Value(m_VertexPositions[i].y);
		root["vtxPos" + iAsString]["z"] = Json::Value(m_VertexPositions[i].z);

		root["vtxNrm" + iAsString]["x"] = Json::Value(m_VertexNormals[i].x);
		root["vtxNrm" + iAsString]["y"] = Json::Value(m_VertexNormals[i].y);
		root["vtxNrm" + iAsString]["z"] = Json::Value(m_VertexNormals[i].z);
	}

	root["faceNormal"]["x"] = Json::Value(m_FaceNormal.x);
	root["faceNormal"]["y"] = Json::Value(m_FaceNormal.y);
	root["faceNormal"]["z"] = Json::Value(m_FaceNormal.z);
	
	root["unkFloat"] = m_unknownFloat;


	root["colProperties"]["polyMaterial"] = Json::Value(m_ColProperties.polyMaterial);
	root["colProperties"]["isLadder"] = Json::Value(m_ColProperties.isLadder);
	root["colProperties"]["ignorePointer"] = Json::Value(m_ColProperties.ignorePointer);


	root["soundProperties"]["soundMaterial"] = Json::Value(m_SoundProperties.soundMaterial);
	root["soundProperties"]["soundEchoSwitch"] = Json::Value(m_SoundProperties.soundEchoSwitch);
}

void Face::JSONDeserialize(Json::Value& root) {
	for (int i = 0; i < 3; i++) {
		std::string vtxPosName = "vtxPos" + std::to_string(i);
		std::string vtxNrmName = "vtxNrm" + std::to_string(i);
		m_VertexPositions[i] = sf::Vector3f(root[vtxPosName]["x"].asFloat(), root[vtxPosName]["y"].asFloat(), root[vtxPosName]["z"].asFloat());
		m_VertexNormals[i] = sf::Vector3f(root[vtxNrmName]["x"].asFloat(), root[vtxNrmName]["y"].asFloat(), root[vtxNrmName]["z"].asFloat());
	}

	m_FaceNormal = sf::Vector3f(root["faceNormal"]["x"].asFloat(), root["faceNormal"]["y"].asFloat(), root["faceNormal"]["z"].asFloat());
	m_unknownFloat = root["unkFloat"].asFloat();

	m_ColProperties.polyMaterial = root["colProperties"]["polyMaterial"].asInt();
	m_ColProperties.isLadder = root["colProperties"]["isLadder"].asBool();
	m_ColProperties.ignorePointer = root["colProperties"]["ignorePointer"].asBool();

	m_SoundProperties.soundMaterial = root["soundProperties"]["soundMaterial"].asInt();
	m_SoundProperties.soundEchoSwitch = root["soundProperties"]["soundEchoSwitch"].asInt();
}