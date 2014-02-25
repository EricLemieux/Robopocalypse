#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include "OBJModel.h"
#include "GameObject.h"
#include "SceneGraph.h"

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

//private:
	std::vector<GameObject*> loadedObjects;
};