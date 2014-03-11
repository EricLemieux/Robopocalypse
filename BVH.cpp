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
	int currentIndex	= 0;
	int targetIndex		= 0;

	std::ifstream file;
	file.open(filePath);

	//Error if the file didnt open properly
	if (!file.is_open())
	{
		std::cout << "Error opening the BVH file " << filePath << std::endl;
		return BVH_H_LOAD_ERROR;
	}
	
	char currentWord[256];
	
	while (!file.eof())
	{
		file >> currentWord;

		//This is the root node of the tree, everything else is attached to this 
		if (!_stricmp(currentWord, "ROOT"))
		{
			//Generate a new node
			Node* currentNode = new Node;

			//Create the joint
			Joint currentJoint;

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

			//Add the node to the vector
			nodeTree.push_back(currentJoint);
		}

		

		if (!_stricmp(currentWord, "JOINT"))
		{
			//Generate a new node
			Node* currentNode = new Node;

			//Create the joint
			Joint currentJoint;

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

				//Skip to the end of the line because we can assume the rest of the info based on the number of chanels
				//file.ignore(256, '\n');
				//file >> currentWord;
			}

			if (!_stricmp(currentWord, "}"))
			{
				currentIndex--;
				targetIndex--;
			
				file.ignore(256, '\n');
				file >> currentWord;
			}

			nodeTree[targetIndex].node->AttachNode(currentNode);

			//currentNode->UpdateLocalPosition();
			currentJoint.node = currentNode;

			//Add the node to the vector
			nodeTree.push_back(currentJoint);
		}

		if (!_stricmp(currentWord, "{"))
		{
			currentIndex++;
			targetIndex++;
			//file.ignore(256, '\n');
			//file >> currentWord;
		}
		
		if (!_stricmp(currentWord, "}"))
		{
			currentIndex--;
			targetIndex--;
			//file.ignore(256, '\n');
			//file >> currentWord;
		}

		//This part of the file is loading the rotations of joints
		if (!_stricmp(currentWord, "MOTION"))
		{
			nodeTree[0].node->Update();
			break;
		}

		//file.ignore(256, '\n');
	}

	return BVH_H_LOAD_FINE;
}