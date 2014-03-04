#pragma once

#include <glm\glm.hpp>

#include "SceneGraph.h"

#include "BasicShapes.h"
#include "VertexBuffer.h"

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

	void SetVertexBuffer(VertexBuffer *vertBuff);

	//////////
	//GETTERS
	//////////
	Node* GetSceneGraphObject(void);

	CollisionBox* GetCollisionBox(void);

	bool GetIsActive(void);

	VertexBuffer* GetVertexBuffer(void);

private:
	Node *sceneGraphObject;
	
	bool isActive;

	VertexBuffer *VBO;
};