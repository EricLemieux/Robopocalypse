#pragma once

#undef _UNICODE
#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>

#include <glm\glm.hpp>

#include "BVH.h"

class Animation
{
public:
	Animation();
	~Animation();

	void LoadSkinWeights(void);
};

