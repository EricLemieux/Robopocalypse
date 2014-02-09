#include "Engine.h"


OBJModel::OBJModel(){}

OBJModel::OBJModel(const char *modelPath, const char *texurePath)
{
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

	//char *filePath = "cat.jpg";
	ilLoadImage(texurePath);
	ILubyte *bytes = ilGetData();
	if (!bytes)
	{
		std::cout << "error opening image file";

		//Clean up memory
		ilBindImage(0);
		ilDeleteImages(1, &texName);
	}
	else
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//Image is now OpenGL's problem
		ilBindImage(0);
		ilDeleteImages(1, &texName);
	}

	char* firstWord = new char();
	char* data = new char();

	//Open the file
	std::ifstream file;
	file.open(modelPath);
	if (!file.is_open())
	{
		std::cout << "Error opening the file " << modelPath << "\n";
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

			//Y data
			file >> data;
			vertex.y = atof(data);

			//Z data
			file >> data;
			vertex.z = atof(data);

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
	}

	std::vector<float> finalNormals;
	for (unsigned int i = 0; i < facesNormals.size(); ++i)
	{
		finalNormals.push_back(normals[facesNormals[i]].x);
		finalNormals.push_back(normals[facesNormals[i]].y);
		finalNormals.push_back(normals[facesNormals[i]].z);
	}

	//Init and add data
	VBO.Initialize(finalVerts.size() / 3, true, true);
	VBO.AddVerticies(&finalVerts[0]);
	VBO.AddNormals(&finalNormals[0]);
	VBO.AddTexCoords(&finalTexCoords[0]);
}
OBJModel::~OBJModel(){}

void OBJModel::drawOBJ(){
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	//replace?
	glBindTexture(GL_TEXTURE_2D, this->getTex());

	glTranslatef(this->getPosX(), this->getPosY(), this->getPosZ());

	VBO.Render();
	
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

glm::vec3 OBJModel::getVertex(int i){
	return out_vertices[i];
}

glm::vec2 OBJModel::getUV(int i){
	return out_uvs[i];
}

glm::vec3 OBJModel::getNormal(int i){
	return out_normals[i];
}

int OBJModel::getVerSize(){
	return out_vertices.size();
}
int OBJModel::getUVSize(){
	return out_uvs.size();
}
int OBJModel::getNormSize(){
	return out_normals.size();
}

void OBJModel::setPos(float xtemp, float ytemp, float ztemp){
	x = xtemp;
	y = ytemp;
	z = ztemp;
	boundingBox.setPos(xtemp,ytemp,ztemp);
}

float OBJModel::getPosX(){
	return x;
}

float OBJModel::getPosY(){
	return y;
}

float OBJModel::getPosZ(){
	return z;
}

GLfloat OBJModel::getTex(){
	return texture;
}

collisionObjects OBJModel::getHitBox(){
	return boundingBox;
}

void OBJModel::setBoundingBox(collisionObjects newBoundBox)
{
	boundingBox = newBoundBox;
}