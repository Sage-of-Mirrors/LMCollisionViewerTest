#pragma once
#include <cmath>
#include <SFML\OpenGL.hpp>
#include <SFML\System\Vector3.hpp>

struct CollisionProperties {
	int polyMaterial; // This will become an enum once the types are documented
	bool isLadder;
	bool ignorePointer; // What does this do? No idea.
};

struct SoundProperties {
	int soundMaterial; // This will become an enum once the types are documented
	int soundEchoSwitch; // What does this do? Not sure.
};

class Face {
	sf::Vector3f m_VertexPositions[3];
	sf::Vector3f m_VertexNormals[3];
	sf::Vector3f m_FaceNormal;
	float m_unknownFloat;
	CollisionProperties m_ColProperties;
	SoundProperties m_SoundProperties;
	sf::Vector3f renderColor; // Temporary until we get an actual color object
public:
	Face();
	void SetVertexPosition(sf::Vector3f, int);
	void SetVertexNormal(sf::Vector3f, int);
	void SetPolyMaterial(int);
	void SetIsLadder(bool);
	void SetIgnorePointer(bool);
	void SetSoundMaterial(int);
	void SetEchoSwitch(int);
	void CalculateFaceNormal();
	void Render();
	CollisionProperties& GetCollisionProperties();
	SoundProperties& GetSoundProperties();
};