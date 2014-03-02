#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include "OBJModel.h"
#include "GameObject.h"
#include "SceneGraph.h"
#include "Texture.h"

//This class is used to store data from an asset file  
class Assets
{
public:
	Assets();
	~Assets();

	//Load the assets from a specific file
	unsigned int Load(char* fileName);

	//Attach all of the game objects in the list to a scene graph node
	unsigned int AttachAllObjectsToNode(Node *sceneGraphNode);

	//Getters

	//Get the size of the vector
	inline unsigned int GetSize(void){ return loadedObjects.size(); }

	//Get a specific game object from the vector
	GameObject* GetObjectAtIndex(unsigned int index);

	void AttachTextureAtIndex(unsigned int index, GLuint texHandles);

private:
	std::vector<GameObject*> loadedObjects;
};