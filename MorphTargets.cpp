#include "MorphTargets.h"


MorphTargets::MorphTargets()
{
	looping = true;
}


MorphTargets::~MorphTargets()
{
}

void MorphTargets::LoadAnimations(void)
{
	//TODO
	//Load a text file that contains all of the animations
}

void MorphTargets::Update(float dt)
{
	if (dt >= 1.0f)
	{
		dt = 0.0f;

		currentFrame = nextFrame;
		if (nextFrame >= MAX_FRAMES_IN_ANIMATION)
		{
			if (looping)
			{
				nextFrame = 0;
			}
		}
		else
		{
			nextFrame += 1;
		}
	}

	//Load the current frame of the animation
	float *currentFrameVerts	= &animations[currentAnimation][currentFrame].GetVerts()[0];
	unsigned int numVerts		= animations[currentAnimation][currentFrame].GetVerts().size();
	
	//Load the next frame of the animation
	float *nextFrameVerts		= &animations[currentAnimation][currentFrame].GetVerts()[0];

	//create an empty array to store the new values of the verts
	finalVerts = new float[numVerts];

	//Update each of the verts by LERPing between the current and next frame
	for (unsigned int i = 0; i < numVerts; ++i)
	{
		finalVerts[i] = NLERP(currentFrameVerts[i], nextFrameVerts[i], dt);
	}
}

void MorphTargets::SetAnimation(playerAnimations anim)
{
	currentAnimation = anim;
}

float* MorphTargets::GetFinalVerts(void)
{
	return finalVerts;
}