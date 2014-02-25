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

	while (!file.eof())
	{
		static GameObject newObject;

		file >> firstWord;

		//Create a new game object
		if (!_stricmp(firstWord, "new"))
		{
		}

		if (!_stricmp(firstWord, "obj"))
		{
			char objPath[256];
			file >> objPath;

			//Load the obj
			OBJModel newOBJ = OBJModel(objPath);

			//Attach the model to the game object
			newObject.AttachModel(newOBJ.GetVBO());
		}

		if (!_stricmp(firstWord, "pos"))
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
			newObject.SetPosition(newPos);

			//Skip ahead to the end of the line
			file.ignore('\n');
		}

		//Load the game object in to the vector and clean up the memory
		if (!_stricmp(firstWord, "load"))
		{
			//Add the game object to the vector
			loadedObjects.push_back(newObject);
		}
	}
}