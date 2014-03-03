#pragma once

#include <glm\glm.hpp>

#include "SceneGraph.h"

class CollisionBox
{
public:
	CollisionBox();
	CollisionBox(Node *node);
	~CollisionBox();

	bool CheckCollision(CollisionBox *other);

	//////////
	//SETTERS
	//////////
	void SetSceneGraphObject(Node *object);

	//////////
	//GETTERS
	//////////
	Node* GetSceneGraphObject(void);

	CollisionBox* GetCollisionBox(void);

private:
	Node *sceneGraphObject;
};

