#include "GameObject.h"


GameObject::GameObject()
{
	glm::mat4 position		= glm::mat4(1);
	glm::mat4 rotation		= glm::mat4(1);
	glm::mat4 scale			= glm::mat4(1);
}


GameObject::~GameObject()
{
}

void GameObject::Render(void)
{
	VBO->ActivateAndRender();
}

glm::mat4 GameObject::UpdateModelViewProjection(glm::mat4 &projectionMat, glm::mat4 &viewMat)
{
	//Find the model matrix
	modelMatrix = scale * rotation * position;

	//Return the model view projection matrix
	return projectionMat * viewMat * modelMatrix;
}

//////////
//Setters
//////////

//Set the position of the game object in world space
void GameObject::setPosition(glm::vec3 newPos)
{
	position[3] = glm::vec4(newPos, 1.0f);
}
void GameObject::setPosition(glm::mat4 newPosMat)
{
	position = newPosMat;
}

//////////
//ATTACHERS
//////////

//Attach a model to the game object
void GameObject::AttachModel(VertexBuffer *model)
{
	VBO = new VertexBuffer(*model);
}