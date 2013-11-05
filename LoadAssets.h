#ifndef LOAD_ASSETS_H
#define LOAD_ASSETS_H

#include <fstream>
#include <iostream>
#include <vector>

#include "Collisions.h"

class Assets
{
public:
	Assets(void);

	std::vector<OBJModel>  objects;
	std::vector<collisionObjects> boundingBoxes;	//TODO Impliment

	void LoadAssets(char fileName[256]);
private:
	
};

#endif