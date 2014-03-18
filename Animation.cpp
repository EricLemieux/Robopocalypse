#include "Animation.h"


Animation::Animation()
{
}


Animation::~Animation()
{
}

//Load the XML file that is storing the skin weights 
std::vector<skinMesh> LoadSkinWeights(char* filePath)
{
	//skinMesh *mesh;
	//mesh = new skinMesh;
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