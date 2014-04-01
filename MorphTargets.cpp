#include "MorphTargets.h"


MorphTargets::MorphTargets()
{
	currentFrame	= 0;
	nextFrame		= 1;

	looping = true;

	finalVerts = new float;
}


MorphTargets::~MorphTargets()
{
}

void MorphTargets::LoadAnimations(void)
{
	//TODO
	//Load a text file that contains all of the animations

	std::vector<OBJModel> temp;

	this;

	OBJModel frame1 = OBJModel("Resources/Models/Robot.obj", false);
	temp.push_back(frame1);
	OBJModel frame2 = OBJModel("Resources/Models/Robot2.obj", false);
	temp.push_back(frame2);

	animations.push_back(temp);
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

	//These are needed to store the vector before turning it into an array for faster looping through
	std::vector<float> currentFrameVertsVector = animations[0][0].GetVerts();
	std::vector<float> nextFrameVertsVector = animations[0][1].GetVerts();

	//Load the current frame of the animation
	float *currentFrameVerts = &currentFrameVertsVector[0];//= &animations[currentAnimation][currentFrame].GetVerts()[0];
	unsigned int numVerts = animations[0][0].GetVerts().size();//= animations[currentAnimation][currentFrame].GetVerts().size();
	
	//Load the next frame of the animation
	float *nextFrameVerts = &nextFrameVertsVector[0];// &animations[currentAnimation][currentFrame].GetVerts()[0];

	//create an empty array to store the new values of the verts
	finalVerts = new float[numVerts];

	//Update each of the verts by LERPing between the current and next frame
	for (unsigned int i = 0; i < numVerts; ++i)
	{
		finalVerts[i] = LERP(currentFrameVerts[i], nextFrameVerts[i], dt);
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