#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "Collisions.h"
#include "VertexBuffer.h"

class OBJModel{
public:
	//Constructors
	OBJModel();
	OBJModel(const char *path, const char *texPath);

	//Destructor
	~OBJModel();
	
	//draw
	void drawOBJ();

	//////////
	//Setters
	//////////

	//Set the position of the model
	void setPos(float xtemp, float ytemp, float ztemp);
	void setPos(glm::vec3 newPos);

	//Set the bounding box of the model
	void setBoundingBox(collisionObjects newBoundBox);

	//////////
	//Getters
	//////////

	//Get the position of the model
	inline glm::vec3 getPos(void){ return position; }

	//Get the texture handle
	GLfloat getTex();

	//Get the hitbox of the model
	collisionObjects getHitBox();

private:
	glm::vec3 position;
	float xspeed,yspeed,zspeed;
	float xaccel,yaccel,zaccel;
	float xforce,yforce,zforce;
	float orientation;
	float dt;

	//VBO for rendering
	VertexBuffer VBO;

	//Texture handle
	GLuint texture;

	//Bounding box for collisions
	collisionObjects boundingBox;
};


#endif