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

	void SetIsActive(bool active);

	//////////
	//GETTERS
	//////////
	Node* GetSceneGraphObject(void);

	CollisionBox* GetCollisionBox(void);

	bool GetIsActive(void);

private:
	Node *sceneGraphObject;
	
	bool isActive;
};