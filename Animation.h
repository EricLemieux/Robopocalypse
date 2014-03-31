#pragma once

#include <glm\glm.hpp>

#include <iostream>
#include <fstream>

#include <GLFW\glfw3.h>

#undef _UNICODE
#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>

#include "BVH.h"

#define MAX_BONE_SIZE 64

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

class AnimationManager
{
private:
	std::vector<BVH> animations;

	unsigned int currentAnimation;
	
	unsigned int currentFrame;
	unsigned int nextFrame;

	float deltaTime;
	double timeOfLastUpdate;

	bool looping;

	glm::mat4 boneTransformations[MAX_BONE_SIZE];

public:
	AnimationManager();
	~AnimationManager();

	void SetAnimations(std::vector<BVH> newAnimations);

	inline void SetCurrentAnimation(int animationIndex){ currentAnimation = animationIndex; }

	void Update(Node *parent);

	inline glm::mat4* GetBoneTransformations(void){ return boneTransformations; }
};