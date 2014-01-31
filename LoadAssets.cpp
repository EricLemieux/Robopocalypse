//This class loads objects information from text files
//an example is that it will load the characters and the world from assets.txt
//another example is loading of morph targets for the characters from assetsPlayerMorphs.txt

#include "Engine.h"

Assets::Assets()
{
}

void Assets::LoadAssets(char fileName[256])
{
	std::ifstream assetFile;

	assetFile.open(fileName);

	if(!assetFile.is_open())
	{
		std::cout<<"Error opening the file, defaulting 'assets.txt.'\n";
		LoadAssets("assets.txt");
	}

	char firstWord[256];

	while(!assetFile.eof())
	{
		char objectName[256];
		char textureName[256];
		char position[256];
		float x,y,z;

		assetFile>>firstWord;

		if(!_stricmp(firstWord ,"obj"))
		{
			assetFile>>objectName;
			assetFile.ignore(256,'\n');
		}
		if(!_stricmp(firstWord ,"tex"))
		{
			assetFile>>textureName;
			assetFile.ignore(256,'\n');
		}
		if(!_stricmp(firstWord ,"pos"))
		{
			assetFile>>position;
			x = atof(position);

			assetFile>>position;
			y = atof(position);

			assetFile>>position;
			z = atof(position);

			assetFile.ignore(256,'\n');
		}
		if(!_stricmp(firstWord, "store"))
		{
			//Load OBJ
			OBJModel newOBJ(objectName, textureName);

			objects.push_back(newOBJ);

			assetFile.ignore(256,'\n');
		}
		if(!_stricmp(firstWord ,"load"))
		{
			//Load OBJ
			OBJModel newOBJ(objectName, textureName);
			

			//Generate the size of the bounding box
			glm::vec3 size, minPos, maxPos;
			for(unsigned int i = 0; i < newOBJ.getVerSize(); ++i)
			{
				//Find the smallest position value
				if (newOBJ.getVertex(i).x < minPos.x)	{	minPos.x = newOBJ.getVertex(i).x;	}
				if (newOBJ.getVertex(i).y < minPos.y)	{	minPos.y = newOBJ.getVertex(i).y;	}
				if (newOBJ.getVertex(i).z < minPos.z)	{ 	minPos.z = newOBJ.getVertex(i).z;	}
																								
				//Find the largest position value												
				if (newOBJ.getVertex(i).x > maxPos.x)	{	maxPos.x = newOBJ.getVertex(i).x;	}
				if (newOBJ.getVertex(i).y > maxPos.y)	{	maxPos.y = newOBJ.getVertex(i).y;	}
				if (newOBJ.getVertex(i).z > maxPos.z)	{ 	maxPos.z = newOBJ.getVertex(i).z;	}
			}
			size = maxPos - minPos;

			glm::vec3 centerPos;
			centerPos = (maxPos+minPos)/glm::vec3(2,2,2);

			collisionObjects newCollisionObject;
			newCollisionObject.setPos(centerPos.x,centerPos.y,centerPos.z);
			newCollisionObject.setSize(size);

			newOBJ.setBoundingBox(newCollisionObject);

			newOBJ.setPos(x,y,z);

			objects.push_back(newOBJ);

			assetFile.ignore(256,'\n');
		}
		
	}
}