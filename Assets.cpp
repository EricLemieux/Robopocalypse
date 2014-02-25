#include "Assets.h"


Assets::Assets()
{
}


Assets::~Assets()
{
}

unsigned int Assets::Load(char* fileName)
{
	//Open up the file 
	std::ifstream file;
	file.open(fileName);

	//Error if the file didnt open properly
	if (!file.is_open())
	{
		std::cout << "Error opening the asset file "<<fileName<<std::endl;
		return 0;
	}

	char firstWord[256];
	GameObject *newObject = new GameObject;

	while (!file.eof())
	{
		file >> firstWord;

		//Comments
		if (!_stricmp(firstWord, "#") || firstWord[0] == '#')
		{
			//Comment doesnt get used by the loader it is only for people reading the file.

			//Skip ahead to the end of the line
			file.ignore(256,'\n');
		}

		//Create a new game object
		else if (!_stricmp(firstWord, "new"))
		{
			//Reset the game object
			newObject = new GameObject;

			//Skip ahead to the end of the line
			file.ignore(256, '\n');
		}

		else if (!_stricmp(firstWord, "obj"))
		{
			char objPath[256];
			file >> objPath;

			//Attach the model to the game object
			newObject->AttachModel(OBJModel(objPath).GetVBO());

			//Skip ahead to the end of the line
			file.ignore(256, '\n');
		}

		else if (!_stricmp(firstWord, "pos"))
		{
			glm::vec3 newPos;
			char value[256];

			//get the values of the new position
			file >> value;
			newPos.x = atof(value);
			file >> value;
			newPos.y = atof(value);
			file >> value;
			newPos.z = atof(value);

			//Set the game object's position
			newObject->SetPosition(newPos);

			//Skip ahead to the end of the line
			file.ignore(256, '\n');
		}

		//Load the game object in to the vector and clean up the memory
		else if (!_stricmp(firstWord, "load"))
		{
			//Add the game object to the vector
			loadedObjects.push_back(newObject);

			//Reset the game object
			newObject = new GameObject;

			//Skip ahead to the end of the line
			file.ignore(256, '\n');
		}
	}
	return 1;
}

unsigned int Assets::AttachAllObjectsToNode(Node *sceneGraphNode)
{
	for (unsigned int i = 0; i < loadedObjects.size() ; ++i)
	{
		sceneGraphNode->AttachNode(loadedObjects[i]->GetNode());
	}

	return 1;
}