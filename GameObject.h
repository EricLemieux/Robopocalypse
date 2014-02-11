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

	//Render the game object to the screen
	void Render(void);

	//////////
	//ATTACHERS
	//////////

	//Attach a model to the game object
	void AttachModel(VertexBuffer *model);

	//TEMP DELETE
	void AttachTestModel(void);

	//////////
	//Setters
	//////////
	void setPosition(glm::vec3 newPos);
	void setPosition(glm::mat4 newPosMat);

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

