#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "Collisions.h"
#include "VertexBuffer.h"

class OBJModel{
public:
	OBJModel();
	OBJModel(const char *path, const char *texPath);
	~OBJModel();
	
	//draw
	void drawOBJ();


	//getter setters
	glm::vec3 getVertex(int i);
	inline void setVertex(glm::vec3 in, int i){ out_vertices[i] = in; }
	glm::vec2 getUV(int i);
	glm::vec3 getNormal(int i);

	int getVerSize();
	int getUVSize();
	int getNormSize();

	void setPos(float xtemp, float ytemp, float ztemp);
	float getPosX();
	float getPosY();
	float getPosZ();

	GLfloat getTex();
	collisionObjects getHitBox();

	void setBoundingBox(collisionObjects newBoundBox);

private:
	float x,y,z;
	float xspeed,yspeed,zspeed;
	float xaccel,yaccel,zaccel;
	float xforce,yforce,zforce;
	float orientation;
	float dt;

	VertexBuffer VBO;

	GLuint texture;

	collisionObjects boundingBox;

	//replace?
	//sf::Image texMap;
	//std::vector<std::vector<int>> out_faces;
	std::vector<std::vector<int>> out_faces;
	std::vector<glm::vec3> out_vertices;
	std::vector<glm::vec2> out_uvs;
	std::vector<glm::vec3> out_normals;
};


#endif