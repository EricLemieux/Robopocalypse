//This class loads objects information from text files
//an example is that it will load the characters and the world from assets.txt
//another example is loading of morph targets for the characters from assetsPlayerMorphs.txt

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

	void LoadAssets(char fileName[256]);
private:
	
};

#endif