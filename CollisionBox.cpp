#include "CollisionBox.h"


CollisionBox::CollisionBox()
{
	sceneGraphObject = new Node;
}

CollisionBox::CollisionBox(Node *node)
{
	sceneGraphObject = node;
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
	if ((b1pos.x + b1scl.x) > (b2pos.x - b2scl.x) && (b1pos.x - b1scl.x) < (b2pos.x + b2scl.x) ||
		(b1pos.y + b1scl.y) > (b2pos.y - b2scl.y) && (b1pos.y - b1scl.y) < (b2pos.y + b2scl.y) ||
		(b1pos.z + b1scl.z) > (b2pos.z - b2scl.z) && (b1pos.z - b1scl.z) < (b2pos.z + b2scl.z)	)
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