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
	double time = glfwGetTime();

	static const char *folder = "Resources/Animations/";
	char currentIndex[256] = "";

	std::vector<OBJModel> idle;
	for (unsigned int i = 1; i <= 10; ++i)
	{
		char filePath[256] = "";
	
		//Load the file
		strcat_s(filePath, folder);
		strcat_s(filePath, "IdleAnimation/idle");
		_itoa_s(i, currentIndex, 10);
		strcat_s(filePath, currentIndex);
		strcat_s(filePath,".obj");
	
		idle.push_back(OBJModel(filePath, false));
	}
	
	animations.push_back(idle);

	std::vector<OBJModel> move;
	for (unsigned int i = 1; i <= 10; ++i)
	{
		char filePath[256] = "";

		//Load the file
		strcat_s(filePath, folder);
		strcat_s(filePath, "RunAnimation/run");
		_itoa_s(i, currentIndex, 10);
		strcat_s(filePath, currentIndex);
		strcat_s(filePath, ".obj");

		move.push_back(OBJModel(filePath, false));
	}

	animations.push_back(move);

	std::vector<OBJModel> punch;
	for (unsigned int i = 1; i <= 10; ++i)
	{
		char filePath[256] = "";

		//Load the file
		strcat_s(filePath, folder);
		strcat_s(filePath, "PunchAnimation/punch");
		_itoa_s(i, currentIndex, 10);
		strcat_s(filePath, currentIndex);
		strcat_s(filePath, ".obj");

		punch.push_back(OBJModel(filePath, false));
	}

	animations.push_back(punch);

	std::vector<OBJModel> block;
	for (unsigned int i = 1; i <= 10; ++i)
	{
		char filePath[256] = "";

		//Load the file
		strcat_s(filePath, folder);
		strcat_s(filePath, "BlockAnimation/block");
		_itoa_s(i, currentIndex, 10);
		strcat_s(filePath, currentIndex);
		strcat_s(filePath, ".obj");

		block.push_back(OBJModel(filePath, false));
	}

	animations.push_back(block);

	std::vector<OBJModel> stagger;
	for (unsigned int i = 1; i <= 10; ++i)
	{
		char filePath[256] = "";

		//Load the file
		strcat_s(filePath, folder);
		strcat_s(filePath, "StaggerAnimation/stagger");
		_itoa_s(i, currentIndex, 10);
		strcat_s(filePath, currentIndex);
		strcat_s(filePath, ".obj");

		stagger.push_back(OBJModel(filePath, false));
	}

	animations.push_back(stagger);

	std::vector<OBJModel> groundKick;
	for (unsigned int i = 1; i <= 10; ++i)
	{
		char filePath[256] = "";

		//Load the file
		strcat_s(filePath, folder);
		strcat_s(filePath, "GroundKickAnimation/groundkick");
		_itoa_s(i, currentIndex, 10);
		strcat_s(filePath, currentIndex);
		strcat_s(filePath, ".obj");

		groundKick.push_back(OBJModel(filePath, false));
	}

	animations.push_back(groundKick);

	std::vector<OBJModel> airKick;
	for (unsigned int i = 1; i <= 10; ++i)
	{
		char filePath[256] = "";

		//Load the file
		strcat_s(filePath, folder);
		strcat_s(filePath, "DiveKickAnimation/divekick");
		_itoa_s(i, currentIndex, 10);
		strcat_s(filePath, currentIndex);
		strcat_s(filePath, ".obj");

		airKick.push_back(OBJModel(filePath, false));
	}

	animations.push_back(airKick);
}

void MorphTargets::Update(float *dt)
{
	if (*dt >= 1.0f)
	{
		*dt = 0.0f;

		currentFrame = nextFrame;
		if (nextFrame >= MAX_FRAMES_IN_ANIMATION-1)
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
	std::vector<float> currentFrameVertsVector	= animations[currentAnimation][currentFrame].GetVerts();
	std::vector<float> nextFrameVertsVector		= animations[currentAnimation][nextFrame].GetVerts();

	//Load the current frame of the animation
	float *currentFrameVerts	= &currentFrameVertsVector[0];
	unsigned int numVerts		= currentFrameVertsVector.size();
	
	//Load the next frame of the animation
	float *nextFrameVerts = &nextFrameVertsVector[0];

	//create an empty array to store the new values of the verts
	finalVerts = new float[numVerts];

	//Update each of the verts by LERPing between the current and next frame
	for (unsigned int i = 0; i < numVerts; ++i)
	{
		finalVerts[i] = LERP(currentFrameVerts[i], nextFrameVerts[i], *dt);
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