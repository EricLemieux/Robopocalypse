#pragma once

#include <glm\glm.hpp>

#include <iostream>
#include <fstream>

#undef _UNICODE
#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>

#include "BVH.h"

struct IDval
{
	float value;
	unsigned int ID;
};

class skinMesh
{
public:
	float boneInfluenceIDs[4];
	float weights[4];

	skinMesh()
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			boneInfluenceIDs[i] = 0.0f;
			weights[i]			= 0.0f;
		}
	}
};

std::vector<skinMesh> LoadSkinWeightsXML(char* filePath);

std::vector<skinMesh> LoadSkinWeightsIMG(char* filePath, std::vector<glm::vec2> texcoords);