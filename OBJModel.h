#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#undef _UNICODE
#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>

#include "VertexBuffer.h"

class OBJModel{
public:
	//Constructors
	OBJModel();
	OBJModel(const char *path);

	//Destructor
	~OBJModel();

	//draw
	void DrawOBJ();

	//////////
	//Setters
	//////////


	//////////
	//Getters
	//////////

	//Get the VBO to attach to a game object
	VertexBuffer* GetVBO(void);

	//Get the texture handle
	GLfloat GetTex();
	
	//Get the max and min vectors
	//Used for collision box
	inline glm::vec3 GetMaxVect(void){ return maxVect; }
	inline glm::vec3 GetMinVect(void){ return minVect; }

private:
	//Max and min vectors
	//used for the generation of the collision box
	glm::vec3 maxVect;
	glm::vec3 minVect;

	//VBO for rendering
	VertexBuffer *VBO;

	//Texture handle
	GLuint texture;
};
