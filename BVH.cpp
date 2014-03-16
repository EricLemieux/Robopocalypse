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

int BVH::BuildSceneGraph()
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
				}

				else if (!_stricmp(currentWord, "CHANNELS"))
				{
					file >> currentWord;
					currentJoint.numChanels = atoi(currentWord);

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

				file.ignore(256, '\n');
				file >> currentWord;
			}

			if (!_stricmp(currentWord, "CHANNELS"))
			{
				file >> currentWord;
				currentJoint.numChanels = atoi(currentWord);
			}

			//Setting the ID of the node
			currentJoint.ID = currentIndex;
			currentNode->ID = currentIndex;

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