#include "OBJModel.h"

OBJModel::OBJModel(){}

OBJModel::OBJModel(const char *modelPath, bool useBones)
{
	VBO = new VertexBuffer;

	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

	std::vector<int> facesVerts;
	std::vector<int> facesTexCoords;
	std::vector<int> facesNormals;

	//load image
	ILuint texName;
	ilGenImages(1, &texName);
	ilBindImage(texName);

	char* firstWord = new char();
	char* data = new char();

	//Open the file
	std::ifstream file;
	file.open(modelPath);
	if (!file.is_open())
	{
		std::cout << "Error opening the file " << modelPath << "\n";
		return;
	}

	//Load data
	while (!file.eof())
	{
		file >> firstWord;

		//Vertex data
		if (!_stricmp(firstWord, "v"))
		{
			glm::vec3 vertex;

			//X data
			file >> data;
			vertex.x = atof(data);

			//check max and min
			if (vertex.x > maxVect.x)
				maxVect.x = vertex.x;
			else if (vertex.x < maxVect.x)
				minVect.x = vertex.x;

			//Y data
			file >> data;
			vertex.y = atof(data);

			//check max and min
			if (vertex.y > maxVect.y)
				maxVect.y = vertex.y;
			else if (vertex.y < maxVect.y)
				minVect.y = vertex.y;

			//Z data
			file >> data;
			vertex.z = atof(data);

			//check max and min
			if (vertex.z > maxVect.z)
				maxVect.z = vertex.z;
			else if (vertex.z < maxVect.z)
				minVect.z = vertex.z;

			//Add to verticies list
			verts.push_back(vertex);
		}
		//Tex coord data
		else if (!_stricmp(firstWord, "vt"))
		{
			glm::vec2 texCoord;

			//X data
			file >> data;
			texCoord.x = atof(data);

			//Y data
			file >> data;
			texCoord.y = atof(data);

			texCoords.push_back(texCoord);
		}

		//normals data
		else if (!_stricmp(firstWord, "vn"))
		{
			glm::vec3 normal;

			//X data
			file >> data;
			normal.x = atof(data);

			//Y data
			file >> data;
			normal.y = atof(data);

			//Z data
			file >> data;
			normal.z = atof(data);

			normals.push_back(normal);
		}

		//faces data
		else if (!_stricmp(firstWord, "f"))
		{
			for (unsigned int i = 0; i < 3; ++i)
			{
				int facesArray[3];

				for (unsigned int i = 0; i < 3; ++i)
				{
					if (i == 2)
						file >> data;
					else
						file.getline(data, 256, '/');
					facesArray[i] = atoi(data);
				}

				facesVerts.push_back(facesArray[0] - 1);
				facesTexCoords.push_back(facesArray[1] - 1);
				facesNormals.push_back(facesArray[2] - 1);
			}
		}

		//skip to the end of the line
		file.ignore(256, '\n');
	}

	//create vertex data for faces
	//float* finalVerts[];
	std::vector<float> finalVerts;
	for (unsigned int i = 0; i < facesVerts.size(); ++i)
	{
		finalVerts.push_back(verts[facesVerts[i]].x);
		finalVerts.push_back(verts[facesVerts[i]].y);
		finalVerts.push_back(verts[facesVerts[i]].z);
	}

	std::vector<float> finalTexCoords;
	for (unsigned int i = 0; i < facesTexCoords.size(); ++i)
	{
		finalTexCoords.push_back(texCoords[facesTexCoords[i]].x);
		finalTexCoords.push_back(texCoords[facesTexCoords[i]].y);

		this->texcoords.push_back(glm::vec2(texCoords[facesTexCoords[i]].x, texCoords[facesTexCoords[i]].y));
	}

	std::vector<float> finalNormals;
	for (unsigned int i = 0; i < facesNormals.size(); ++i)
	{
		finalNormals.push_back(normals[facesNormals[i]].x);
		finalNormals.push_back(normals[facesNormals[i]].y);
		finalNormals.push_back(normals[facesNormals[i]].z);
	}

	this->verts = finalVerts;
	
	//Init and add data
	VBO->Initialize(facesVerts.size(), true, true, useBones);
	VBO->AddVerticies(&finalVerts[0]);
	VBO->AddNormals(&finalNormals[0]);
	VBO->AddTexCoords(&finalTexCoords[0]);
}
OBJModel::~OBJModel(){}

void OBJModel::DrawOBJ()
{
	VBO->ActivateAndRender();
	
	glDisable(GL_TEXTURE_2D);
}


//Get the texture handle
GLfloat OBJModel::GetTex()
{
	return texture;
}

//Get the VBO
VertexBuffer* OBJModel::GetVBO(void)
{
	return VBO;
}

std::vector<float> OBJModel::GetVerts(void)
{
	return verts;
}