#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "Engine.h"

class OBJModel{
public:
	OBJModel();
	OBJModel(const char *path, const char *texPath);
	~OBJModel();
	
	//movement



	//getter setters
	sf::Vector3f getVertex(int i);
	sf::Vector2f getUV(int i);
	sf::Vector3f getNormal(int i);

	int getVerSize();
	int getUVSize();
	int getNormSize();

	void setPos(float xtemp, float ytemp, float ztemp);
	float getPosX();
	float getPosY();
	float getPosZ();

	GLfloat getTex();

private:
	float x,y,z;
	float xspeed,yspeed,zspeed;
	float orientation;
	float dt;
	GLuint texture;

	sf::Image texMap;
	//std::vector<std::vector<int>> out_faces;
	std::vector<std::vector<int>> out_faces;
	std::vector<sf::Vector3f> out_vertices;
	std::vector<sf::Vector2f> out_uvs;
	std::vector<sf::Vector3f> out_normals;
};


#endif