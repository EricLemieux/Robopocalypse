#pragma once

//////////
//INCLUDES
//////////
#include <glm\glm.hpp>

#include "OBJModel.h"
#include "VertexBuffer.h"
#include "SceneGraph.h"
#include "Texture.h"

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

	//Attach a texture to the game object
	void AttachTexture(GLuint tex);

	//Attach a normal map to the game object
	void AttachNormalMap(GLuint nMap);
	
	//////////
	//Setters
	//////////
	void SetPosition(glm::vec3 newPos);
	void MovePosition(glm::vec3 translation);

	void SetSceneGraphChild(Node *child);

	//////////
	//GETTERS
	//////////
	inline Node* GetNode(void){ return sceneGraphObject; }

	inline GLuint GetTextureHandle(void){ return textureHandle; }
	inline GLuint GetNormalMapHandle(void){ return normalMapHandle; }

private:
	//////////
	//DATA
	//////////
	
	Node *sceneGraphObject;

	VertexBuffer *VBO;

	GLuint textureHandle;
	GLuint normalMapHandle;
};

