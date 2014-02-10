#include "GameObject.h"


GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}


glm::mat4 GameObject::UpdateModelViewProjection(glm::mat4 &projectionMat, glm::mat4 &viewMat)
{
	//Find the model matrix
	modelMatrix = scale * rotation * position;

	//Return the model view projection matrix
	return projectionMat * viewMat * modelMatrix;
}