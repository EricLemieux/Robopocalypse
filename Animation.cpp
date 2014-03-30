#include "Animation.h"

//Load the XML file that is storing the skin weights 
std::vector<skinMesh> LoadSkinWeightsXML(char* filePath)
{
	std::vector<skinMesh> mesh;

	std::ifstream file;
	file.open(filePath);

	//Error opening the file
	if (!file.is_open())
	{
		std::cout << "Mesh skin weight XML file "<<filePath<<" failed to load.";
		return mesh;
	}

	char* currentWord;
	currentWord = new char;

	int vSize = 0;

	//skip to the start of the shape data
	bool keepLookingForSize = true;
	while (_stricmp(currentWord, "<shape") && keepLookingForSize)
	{
		file.ignore(512, ' ');
		char c = file.get();

		if(c == 's' && file.peek() == 'i')
		{
			file.ignore(512, '"');

			std::vector<char> str;

			while(true)
			{
				char i = file.get();
				if(i >= '0' && i <= '9')
				{
					str.push_back(i);
				}
				else
				{
					keepLookingForSize = false;
					break;
				}
			}

			vSize = atoi(&str[0]);
		}
	}

	//Fill up the vector with empty values, so we can change them later
	for(unsigned int i = 0; i < vSize; ++i)
	{
		skinMesh temp;
		mesh.push_back(temp);
	}

	//Skip the rest of the shape data
	while (_stricmp(currentWord, "</shape>"))
	{
		file.ignore(512, '\n');
		file >> currentWord;
	}

	//Read until the end of the file
	while (!file.eof())
	{
		bool doneReadingWeights = false;

		unsigned int size  = 0;
		unsigned int layer = 0;
		unsigned int index = 0;

		//Collect attribute data from the start tag
		{
			//Skip to the start of the new start tag
			file.ignore(512, '<');
			file >> currentWord;

			if (!_stricmp(currentWord, "weights"))
			{
				//Get the first space then start skipping the parts we arent using
				file.get();

				//Ignore deformer name
				file.ignore(512, ' ');

				//Ignore Source name, already have in .bvh
				file.ignore(512, ' ');

				//Ignore Shape name
				file.ignore(512, ' ');

				//Get the layer ID
				file.ignore(512, '"');
				unsigned int sizeOfLayerID = 0;
				while (true)
				{
					char c = file.peek();
					if (c >= '0' && c <= '9')
					{
						c = file.get();

						if (sizeOfLayerID >= 1)
						{
							layer *= 10;
						}

						layer += atoi(&c);
						sizeOfLayerID++;
					}
					else
					{
						break;
					}
				}

				//Ignore default value
				file.ignore(512, ' ');
				file.ignore(512, ' ');

				//Get the Size
				file.ignore(512, '"');
				unsigned int sizeOfSize = 0;	//Yo dawg we heard you like size...
				while (true)
				{
					char c = file.peek();
					if (c >= '0' && c <= '9')
					{
						c = file.get();

						if (sizeOfSize >= 1)
						{
							size *= 10;
						}

						size += atoi(&c);
						sizeOfSize++;
					}
					else
					{
						break;
					}
				}

				//Ignore the rest of the line
				file.ignore(512, '\n');
			}
		}

		//Collect the index and weight values from the body
		while (!doneReadingWeights)
		{
			if (size == 0)
			{
				doneReadingWeights = true;
				break;
			}

			file >> currentWord;

			if (!_stricmp(currentWord,"<point"))
			{
				//Get index of the vertex
				file.ignore(512, '"');
				unsigned int sizeOfIndex = 0;
				int index = 0;
				while (true)
				{
					char c = file.peek();
					if (c >= '0' && c <= '9')
					{
						c = file.get();

						if (sizeOfIndex >= 1)
						{
							index *= 10;
						}

						index += atoi(&c);
						sizeOfIndex++;
					}
					else
					{
						break;
					}
				}

				//Read the weight value
				std::vector<char> str;
				file.ignore(512, ' ');
				file.ignore(512, '"');
				while (true)
				{
					char c = file.peek();
					if ((c >= '0' && c <= '9') || c == '.')
					{
						c = file.get();
						str.push_back(c);
					}
					else
					{
						break;
					}
				}
				float value = atof(&str[0]);

				//Skip to the end of the line.
				file.ignore(256, '\n');

				//Apply the values of the data that we just collected.
				for (unsigned int i = 0; i < 4; ++i)
				{
					if (mesh[index].weights[i] == 0.0f)
					{
						mesh[index].boneInfluenceIDs[i]	= layer;
						mesh[index].weights[i]			= value;
						break;
					}
				}

			}
			else if (!_stricmp(currentWord, "</weights>"))
			{
				//The end of the weight tag
				int a = 0;
				doneReadingWeights = true;
			}
			else
			{
				//Something went wrong you shouldn't be reading data from here.
				std::cout << "Error loading XML file.";
				return mesh;
			}
		}
	}

	return mesh;
}

std::vector<skinMesh> LoadSkinWeightsIMG(char* filePath, std::vector<glm::vec2> texcoords)
{
	std::vector<skinMesh> mesh;

	//Load the file that stores the different skin weight maps
	std::fstream file;
	file.open(filePath);

	if (!file.is_open())
	{
		std::cout << "Error opening the skin weight manager file.\n See function LoadSkinWeightsIMG() in animation.cpp for more details.\n";
		return mesh;
	}

	char *currentWord = new char;
	std::vector<std::string> weightMapsFilePaths;

	//Read the manger file to know what files to open and check the values for the weight maps
	while (!file.eof())
	{
		file >> currentWord;

		//this line is a comment skip it
		if (!_stricmp(currentWord, "//") || !_stricmp(currentWord, ""))
		{
			file.ignore(256, '\n');
		}
		else
		{
			//Get the name of the joint
			std::string jointName;
			file >> jointName;

			//Get the file path to the image that stores the weights
			std::string imageFilePath;
			file >> imageFilePath;

			//Concatenate the folder directory into the file path
			std::string finalFilePath;
			finalFilePath = std::string("Resources\\Bones\\skin\\" + std::string(imageFilePath));

			//Add this file path to the list of file paths.
			weightMapsFilePaths.push_back(finalFilePath);
			int a = 0;
		}
	}

	//vect of vects that stores all of the values of the colour for each of the vectors.
	std::vector<std::vector<float>> vertStorage;

	//Loop through each of the textures
	for (unsigned int i = 0; i < weightMapsFilePaths.size(); ++i)
	{
		//Load the image using DevIL
		ILuint *tex;
		tex = new ILuint;
		ilGenImages(1, tex);
		ilBindImage(*tex);

		ilLoadImage(weightMapsFilePaths[i].c_str());
		//ILubyte *data = ilGetData();

		ILuint width, height;
		width = ilGetInteger(IL_IMAGE_WIDTH);
		height = ilGetInteger(IL_IMAGE_HEIGHT);

		//Store 
		std::vector<float> values;
	
		//Get the value of the pixel at the UV coordinate
		for (unsigned int j = 0; j < texcoords.size(); ++j)
		{
			ILuint data;
			ilCopyPixels(texcoords[j].x * width, /*1.0f -*/texcoords[j].y*height, 0, 1, 1, 1, IL_RGBA, IL_UNSIGNED_BYTE, &data);
			int r, g, b, a;
			a = (data >> 24) & 0xFF;
			b = (data >> 16) & 0xFF;
			g = (data >> 8) & 0xFF;
			r = (data)& 0xFF;

			float value = (float)g/255.0f;

			value += 0.0001f;

			values.push_back(value);			
		}

		vertStorage.push_back(values);

		//Remove the texture from memory.
		ilDeleteImage(*tex);
		delete tex;
	}

	//Loop through each vertex to store the top 4 weight values per vertex.
	for (unsigned int i = 0; i < texcoords.size(); ++i)
	{
		skinMesh tempMesh;

		//Loop through the data collected from the textures
		for (unsigned int j = 0; j < vertStorage.size(); ++j)
		{
			if (vertStorage[j][i] >= tempMesh.weights[0])
			{
				tempMesh.weights[3]				= tempMesh.weights[2];
				tempMesh.boneInfluenceIDs[3]	= tempMesh.boneInfluenceIDs[2];
			
				tempMesh.weights[2]				= tempMesh.weights[1];
				tempMesh.boneInfluenceIDs[2]	= tempMesh.boneInfluenceIDs[1];
			
				tempMesh.weights[1]				= tempMesh.weights[0];
				tempMesh.boneInfluenceIDs[1]	= tempMesh.boneInfluenceIDs[0];
			
				tempMesh.weights[0]				= vertStorage[j][i];
				tempMesh.boneInfluenceIDs[0]	= j;
			}
			else if (vertStorage[j][i] >= tempMesh.weights[1])
			{
				tempMesh.weights[3] = tempMesh.weights[2];
				tempMesh.boneInfluenceIDs[3] = tempMesh.boneInfluenceIDs[2];
			
				tempMesh.weights[2] = tempMesh.weights[1];
				tempMesh.boneInfluenceIDs[2] = tempMesh.boneInfluenceIDs[1];
			
				tempMesh.weights[1] = vertStorage[j][i];
				tempMesh.boneInfluenceIDs[1] = j;
			}
			else if (vertStorage[j][i] >= tempMesh.weights[2])
			{
				tempMesh.weights[3] = tempMesh.weights[2];
				tempMesh.boneInfluenceIDs[3] = tempMesh.boneInfluenceIDs[2];
			
				tempMesh.weights[2] = vertStorage[j][i];
				tempMesh.boneInfluenceIDs[2] = j;
			}
			else if (vertStorage[j][i] >= tempMesh.weights[3])
			{
				tempMesh.weights[3] = vertStorage[j][i];
				tempMesh.boneInfluenceIDs[3] = j;
			}
		}
		mesh.push_back(tempMesh);
	}

	return mesh;
}

//ANIMATION MANAGER CLASS

AnimationManager::AnimationManager()
{
	currentAnimation = 0;
	currentFrame = 0;
	nextFrame = 0;
	looping = true;

	timeOfLastUpdate = 0.0f;
}
AnimationManager::~AnimationManager()
{

}

void AnimationManager::SetAnimations(std::vector<BVH> newAnimations)
{
	animations = newAnimations;
}

void AnimationManager::Update(void)
{
	//Update the current and next frames
	currentFrame = nextFrame;
	if (nextFrame+1 >= animations[currentAnimation].GetNumFrames())
	{
		if (looping)
		{
			nextFrame = 0;
		}
	}
	else
	{
		nextFrame++;
	}
	
	glm::mat4 temp = glm::mat4(1.0f);

	Joint *animation;
	std::vector<Joint> NT = animations[currentAnimation].GetNodeTree();
	animation = &NT[0];

	//Get the transformation matrices for all joints
	unsigned int size = animations[currentAnimation].GetNodeTree().size();
	for (unsigned int i = 0; i < size; ++i)
	{
		temp = glm::mat4(1.0f);
	
		//Rotate
		temp = glm::rotate(temp, animation[i].rotationChanges[currentFrame].z, glm::vec3(0, 0, 1));
		temp = glm::rotate(temp, animation[i].rotationChanges[currentFrame].x, glm::vec3(1, 0, 0));
		temp = glm::rotate(temp, animation[i].rotationChanges[currentFrame].y, glm::vec3(0, 1, 0));
		
		animation[i].node->SetRotation(temp);
		
		temp[3] = glm::vec4(animation[i].positionChanges[currentFrame], 1);
	
		animations[currentAnimation].GetRootNode()->Update();
		
		 glm::mat4 worldRotation = animation[i].node->GetWorldTransform();
		 worldRotation[3] = glm::vec4(0, 0, 0, 1);
		 boneTransformations[i] = worldRotation * glm::inverse(animation[i].offset);
	}
}