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
		if(!_stricmp(firstWord ,"load"))
		{
			//Load OBJ
			OBJModel newOBJ(objectName, textureName);
			newOBJ.setPos(x,y,z);

			objects.push_back(newOBJ);

			collisionObjects newBoundBox;
			newBoundBox.setPos(x,y,z);
			newBoundBox.setSize(5.0f,10.0f,10.0f);

			boundingBoxes.push_back(newBoundBox);

			assetFile.ignore(256,'\n');
		}
		
	}
}