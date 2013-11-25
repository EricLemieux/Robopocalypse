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
			////glm::vec3 pos = minPos + size/glm::vec3(2);
			//glm::mat4 trans(1.0f);
			//trans[3] = glm::vec4(x,y,z,1);
			//glm::mat4 min(1), max(1);
			//min[3] = glm::vec4(minPos,1);
			//max[3] = glm::vec4(maxPos,1);
			//glm::mat4 pos = (min * max)/glm::mat4(2.0f) * trans*glm::mat4(1.0f);

			glm::vec3 centerPos;
			centerPos = (maxPos+minPos)/glm::vec3(2,2,2);

			collisionObjects newCollisionObject;
			newCollisionObject.setPos(centerPos.x,centerPos.y,centerPos.z);
			newCollisionObject.setSize(size);

			newOBJ.setBoundingBox(newCollisionObject);

			objects.push_back(newOBJ);

			////Generate the size of the bounding box
			//glm::vec3 size, minPos, maxPos;
			//for(unsigned int i = 0; i < newOBJ.getVerSize(); ++i)
			//{
			//	//Find the smallest position value
			//	if (newOBJ.getVertex(i).x < minPos.x)	{	minPos.x = newOBJ.getVertex(i).x;	}
			//	if (newOBJ.getVertex(i).y < minPos.y)	{	minPos.y = newOBJ.getVertex(i).y;	}
			//	if (newOBJ.getVertex(i).z < minPos.z)	{ 	minPos.z = newOBJ.getVertex(i).z;	}
			//																					
			//	//Find the largest position value												
			//	if (newOBJ.getVertex(i).x > maxPos.x)	{	maxPos.x = newOBJ.getVertex(i).x;	}
			//	if (newOBJ.getVertex(i).y > maxPos.y)	{	maxPos.y = newOBJ.getVertex(i).y;	}
			//	if (newOBJ.getVertex(i).z > maxPos.z)	{ 	maxPos.z = newOBJ.getVertex(i).z;	}
			//}
			//size = maxPos - minPos;
			////glm::vec3 pos = minPos + size/glm::vec3(2);
			//glm::mat4 trans(1.0f);
			//trans[3] = glm::vec4(x,y,z,1);
			//glm::mat4 min(1), max(1);
			//min[3] = glm::vec4(minPos,1);
			//max[3] = glm::vec4(maxPos,1);
			//glm::mat4 pos = (min * max)/glm::mat4(2.0f) * trans*glm::mat4(1.0f);
			////glm::vec3 pos = (minPos + maxPos)/glm::vec3(2);


			//collisionObjects newBoundBox;
			//newBoundBox.setPos(x,y,z);
			//newBoundBox.setSize(5.0f,10.0f,10.0f);
			//newBoundBox.setPos(pos[3].x,pos[3].y,pos[3].z);
			//newBoundBox.setSize(size.x, size.y, size.z);


			//boundingBoxes.push_back(newBoundBox);

			assetFile.ignore(256,'\n');
		}
		
	}
}