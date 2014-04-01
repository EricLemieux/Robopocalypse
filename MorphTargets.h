#pragma once

#include <vector>

#include "OBJModel.h"
#include "AnimationMath.h"

#include <GLFW\glfw3.h>

#define MAX_FRAMES_IN_ANIMATION 10

enum playerAnimations
{
	PLAYER_ANIMATION_IDLE = 0,
	PLAYER_ANIMATION_MOVE,
	PLAYER_ANIMATION_PUNCH,
	PLAYER_ANIMATION_BLOCK,
	//etc...
};
class MorphTargets
{
private:
	unsigned int currentFrame;
	unsigned int nextFrame;

	bool looping;

	playerAnimations currentAnimation;

	std::vector<std::vector<OBJModel>> animations;

	float *finalVerts;

public:
	MorphTargets();
	~MorphTargets();

	void LoadAnimations(void);

	void Update(float *dt);

	void SetAnimation(playerAnimations anim);

	float* GetFinalVerts(void);
};