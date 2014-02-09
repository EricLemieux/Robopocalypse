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

	//Set the rotation of the model
	inline void setRotation(glm::vec3 newRotation){ rotation = newRotation; }

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

	//Get the max and min vectors
	//Used for collision box
	inline glm::vec3 getMaxVect(void){ return maxVect; }
	inline glm::vec3 getMinVect(void){ return minVect; }
	
	//////////
	//Updaters
	//////////
	glm::mat4 updateModelMatrix(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix);

private:
	glm::vec3 position;
	glm::vec3 rotation;

	float xspeed,yspeed,zspeed;
	float xaccel,yaccel,zaccel;
	float xforce,yforce,zforce;
	float orientation;
	float dt;

	//Max and min vectors
	//used for the generation of the collision box
	glm::vec3 maxVect;
	glm::vec3 minVect;

	//VBO for rendering
	VertexBuffer VBO;

	//Texture handle
	GLuint texture;

	//Bounding box for collisions
	collisionObjects boundingBox;
};


#endif