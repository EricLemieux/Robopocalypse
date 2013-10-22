#ifndef LOAD_ASSETS_H
#define LOAD_ASSETS_H

#include "Engine.h"

#include <fstream>
#include <iostream>
#include <vector>


class Assets
{
public:
	Assets(void);

	std::vector<OBJModel>  objects;

	void LoadAssets(char fileName[256]);
private:
	
};

#endif