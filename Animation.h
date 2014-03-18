#pragma once

#include <glm\glm.hpp>

#include <iostream>
#include <fstream>

#include "BVH.h"

class skinMesh
{
public:
	float boneInfluenceIDs[4];
	float weights[4];

	inline skinMesh()
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			boneInfluenceIDs[i] = 0.0f;
			weights[i]			= 0.0f;
		}
	}
};

class Animation
{
public:
	Animation();
	~Animation();
};

skinMesh* LoadSkinWeights(char* filePath);