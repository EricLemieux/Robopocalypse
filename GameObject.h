#pragma once

//////////
//INCLUDES
//////////
#include <glm\glm.hpp>

#include "OBJModel.h"
#include "VertexBuffer.h"

//////////
//GAME OBJECT CLASS
//
//This is the basic class that is used for everyevery object used in the game
//Some uses include: cameras, lights, players, etc..
//////////
class GameObject
{
public:
	//////////
	//CONSTRUCTORS
	//////////
	GameObject();
	~GameObject();

	glm::mat4 UpdateModelViewProjection(glm::mat4 &projectionMat, glm::mat4 &viewMat);

	//////////
	//ATTACHERS
	//////////

	//Attach a model to the game object
	inline void AttachModel(VertexBuffer *model){ VBO = model; }

private:
	//////////
	//DATA
	//////////

	//
	glm::mat4 position;
	glm::mat4 rotation;
	glm::mat4 scale;
	glm::mat4 modelMatrix;

	VertexBuffer *VBO;
};

