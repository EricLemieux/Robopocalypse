#include "CollisionBox.h"


CollisionBox::CollisionBox()
{
	sceneGraphObject = new Node;

	isActive = false;
}

CollisionBox::CollisionBox(Node *node)
{
	sceneGraphObject = node;
	
	isActive = false;
}


CollisionBox::~CollisionBox()
{
}

bool CollisionBox::CheckCollision(CollisionBox *other)
{
	glm::vec3 b1pos = sceneGraphObject->GetWorldPosition();
	glm::vec3 b1scl = sceneGraphObject->GetScale();

	glm::vec3 b2pos = other->GetSceneGraphObject()->GetWorldPosition();
	glm::vec3 b2scl = other->GetSceneGraphObject()->GetScale();

	//Checks to see if the box is within the other box
	if (((b1pos.x - b2pos.x) < (b1scl.x + b2scl.x) / 2.0f)	&&
		((b1pos.y - b2pos.y) < (b1scl.y + b2scl.y) / 2.0f)	&&
		((b1pos.z - b2pos.z) < (b1scl.z + b2scl.z) / 2.0f))
	{
		return true;
	}

	return false;
}

//////////
//SETTERS
//////////
void CollisionBox::SetSceneGraphObject(Node *object)
{
	sceneGraphObject = object;
}

void CollisionBox::SetIsActive(bool active)
{
	isActive = active;
}

//////////
//GETTERS
//////////
Node* CollisionBox::GetSceneGraphObject(void)
{
	return sceneGraphObject;
}

CollisionBox* CollisionBox::GetCollisionBox(void)
{
	return this;
}

bool CollisionBox::GetIsActive(void)
{
	return isActive;
}