#include "BVH.h"


BVH::BVH()
{
	this->filePath = new char;

	rootNode = new Node;
}

BVH::BVH(char* filePath)
{
	LoadFile(filePath);

	rootNode = new Node;
}

BVH::~BVH()
{
}

void BVH::LoadFile(char* filePath)
{
	this->filePath = filePath;
}

int BVH::BuildSceneGraph(Node *parent)
{
	unsigned int currentIndex	= 0;
	unsigned int targetIndex	= 0;
	
	unsigned int difference		= 0;

	bool endFlag				= false;

	std::ifstream file;
	file.open(filePath);

	//Error if the file didnt open properly
	if (!file.is_open())
	{
		std::cout << "Error opening the BVH file " << filePath << std::endl;
		return BVH_H_LOAD_ERROR;
	}
	
	char currentWord[256];
	file >> currentWord;

	while (!file.eof())
	{
		//file >> currentWord;

		//This is the root node of the tree, everything else is attached to this 
		if (!_stricmp(currentWord, "ROOT"))
		{
			//Generate a new node
			Node* currentNode = new Node;

			//Create the joint
			Joint currentJoint;

			currentJoint.name = new char;
			file >> currentJoint.name;

			currentIndex = 0;
			targetIndex = -1;

			//loop through the file before it describes the first joint node
			while (_stricmp(currentWord, "JOINT"))
			{
				if (!_stricmp(currentWord, "OFFSET"))
				{
					glm::vec3 pos;

					file >> currentWord;
					pos.x = atof(currentWord);

					file >> currentWord;
					pos.y = atof(currentWord);

					file >> currentWord;
					pos.z = atof(currentWord);

					currentNode->SetLocalPosition(pos);

					glm::mat4 startingPos;
					startingPos[3] = glm::vec4(pos, 1.0f);
					currentJoint.offset = startingPos;
				}

				else if (!_stricmp(currentWord, "CHANNELS"))
				{
					file >> currentWord;
					currentJoint.numChannels = atoi(currentWord);

					//Skip to the end of the line because we can assume the rest of the info based on the number of chanels
					file.ignore(256, '\n');
				}

				file >> currentWord;
			}

			rootNode->AttachNode(currentNode);
			currentJoint.node = currentNode;
			currentJoint.ID = currentIndex;

			//Add the node to the vector
			nodeTree.push_back(currentJoint);
		}

		parent->AttachNode(rootNode);

		if (!_stricmp(currentWord, "JOINT"))
		{
			//Generate a new node
			Node* currentNode = new Node;

			//Create the joint
			Joint currentJoint;

			currentJoint.name = new char;
			file >> currentJoint.name;

			file.ignore(256, '\n');
			file >> currentWord;

			if (!_stricmp(currentWord, "{"))
			{
				currentIndex++;
				targetIndex++;
			
				file.ignore(256, '\n');
				file >> currentWord;
			}

			if (!_stricmp(currentWord, "OFFSET"))
			{
				glm::vec3 pos;

				file >> currentWord;
				pos.x = atof(currentWord);

				file >> currentWord;
				pos.y = atof(currentWord);

				file >> currentWord;
				pos.z = atof(currentWord);

				currentNode->SetLocalPosition(pos);

				glm::mat4 startingPos;
				startingPos[3] = glm::vec4(pos, 1.0f);
				currentJoint.offset = startingPos;

				file.ignore(256, '\n');
				file >> currentWord;
			}

			if (!_stricmp(currentWord, "CHANNELS"))
			{
				file >> currentWord;
				currentJoint.numChannels = atoi(currentWord);
			}

			//Setting the ID of the node
			currentJoint.ID = currentIndex;
			currentNode->SetID(currentIndex);

			unsigned int parentIndex = targetIndex;

			for (unsigned int i = 0; i < difference; ++i)
			{
				parentIndex = nodeTree[parentIndex].node->GetParent()->GetID();
			}

			//Set the scene graph node
			nodeTree[parentIndex].node->AttachNode(currentNode);
			currentJoint.node = currentNode;

			//Add the node to the vector
			nodeTree.push_back(currentJoint);

			//Reset the difference so that only this first node after the End site is affected.
			difference = 0;
		}
				
		if (!_stricmp(currentWord, "End"))
		{
			endFlag = true;

			difference = -1;

			//Skip several lines because we want to get right to the close brackets 
			file.ignore(256, '\n');
			file >> currentWord;

			//Loop through all of the close brackets
			while (endFlag)
			{
				if (!_stricmp(currentWord, "}"))
				{
					difference++;

					file >> currentWord;
				}
				else
				{
					//Only if the line is the start of a joint do we exit the loop.
					if (!_stricmp(currentWord, "JOINT"))
					{
						endFlag = false;
					}
					//This part of the file is loading the rotations of joints we will deal with that in a different function.
					else if(!_stricmp(currentWord, "MOTION"))
					{
						nodeTree[0].node->Update();
						return BVH_H_LOAD_FINE;
					}
					else
					{
						file >> currentWord;
					}
				}
			}			
		}
		else
		{
			file >> currentWord;
		}
	}

	return BVH_H_LOAD_FINE;
}

int BVH::BuildMotionData(void)
{
	//Return an error if there aren't any nodes to apply motion to.
	if (nodeTree.size() < 1)
	{
		return BVH_M_LOAD_ERROR;
	}

	std::ifstream file;
	file.open(filePath);

	//Error if the file didn't open properly
	if (!file.is_open())
	{
		std::cout << "Error opening the BVH file " << filePath << std::endl;
		return BVH_H_LOAD_ERROR;
	}

	char currentWord[256];

	bool reachedMotion = false;

	//Loop until we are at the MOTION section of the file.
	while (!reachedMotion)
	{
		//Input the current word
		file >> currentWord;

		//IF the current word is not 'MOTION'
		if (_stricmp(currentWord, "MOTION"))
		{
			//Skip to the end of the line
			file.ignore(256, '\n');
		}
		else
		{
			//We have reached the MOTION section of the file exit the loop.
			reachedMotion = true;
		}
	}

	//Collect the amount  of frames in the file
	file >> currentWord;
	if (!_stricmp(currentWord, "Frames:"))
	{
		file >> currentWord;
		numFrames = atoi(currentWord);
	}

	//Collect the amount of time between each frame
	file >> currentWord;
	if (!_stricmp(currentWord, "Frame"))
	{
		file >> currentWord;
		if (!_stricmp(currentWord, "Time:"))
		{
			file >> currentWord;
			frameTime = atof(currentWord);
		}
	}

	unsigned int index = 0;

	//Read through the Motion data
	while (!file.eof())
	{
		//Update the right number of channels
		if (nodeTree[index].numChannels == 3)
		{
			//When the node only has 3 channels it only captures the rotations and not the positions
			//It stores the rotations in the format (Z, X, Y)

			glm::vec3 rotation;

			file >> currentWord;
			rotation.z = atof(currentWord);

			file >> currentWord;
			rotation.x = atof(currentWord);

			file >> currentWord;
			rotation.y = atof(currentWord);

			nodeTree[index].rotationChanges.push_back(rotation);
		}
		else if (nodeTree[index].numChannels == 6)
		{
			//When the node has 6 channels it keeps track of the positions as well as the rotations
			//Positions are stored in the format of (X, Y, Z)
			//Rotations are stored in the format of (Z, X, Y)

			//Get positions
			glm::vec3 position;

			file >> currentWord;
			position.x = atof(currentWord);

			file >> currentWord;
			position.y = atof(currentWord);

			file >> currentWord;
			position.z = atof(currentWord);

			nodeTree[index].positionChanges.push_back(position);

			//Get rotations
			glm::vec3 rotation;

			file >> currentWord;
			rotation.z = atof(currentWord);

			file >> currentWord;
			rotation.x = atof(currentWord);

			file >> currentWord;
			rotation.y = atof(currentWord);

			nodeTree[index].rotationChanges.push_back(rotation);
		}
		else
		{
			//There should either be 3 or 6 channels
			//Something went wrong in the hierarchy building
			return BVH_M_LOAD_ERROR;
		}

		//Check if we are at the end of the line
		//and if we are reset the index
		if (index == nodeTree.size()-1)
		{
			index = 0;
		}
		else
		{
			index += 1;
		}
	}

	return BVH_M_LOAD_FINE;
}