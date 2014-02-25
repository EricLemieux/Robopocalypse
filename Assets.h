#pragma once

#include <fstream>
#include <iostream>
#include <vector>

#include "OBJModel.h"
#include "GameObject.h"

//This class is used to store data from an asset file  
class Assets
{
public:
	Assets();
	~Assets();

	//Load the assets from a specific file
	unsigned int Load(char* fileName);

private:
	std::vector<GameObject> loadedObjects;
};

