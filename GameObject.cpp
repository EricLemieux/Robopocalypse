#include "GameObject.h"


GameObject::GameObject()
{
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
	//Return the model view projection matrix
	return projectionMat * viewMat * sceneGraphObject.GetWorldTransform();
}

//////////
//Setters
//////////

//Set the position of the game object in world space
void GameObject::SetPosition(glm::vec3 newPos)
{
	sceneGraphObject.SetLocalPosition(newPos);
}
//Translate the position of the object
void GameObject::MovePosition(glm::vec3 translation)
{
	sceneGraphObject.TranslateNode(translation);
}

//Attach a child object in the scene graph
void GameObject::SetSceneGraphChild(Node *child)
{
	sceneGraphObject.AttachNode(child);
}

//////////
//ATTACHERS
//////////

//Attach a model to the game object
void GameObject::AttachModel(VertexBuffer *model)
{
	VBO = new VertexBuffer(*model);
}