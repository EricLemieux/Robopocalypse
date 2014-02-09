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
	VBO.Initialize(finalVerts.size() / 3, false, false);
	VBO.AddVerticies(&finalVerts[0]);
	VBO.AddNormals(&finalNormals[0]);
	VBO.AddTexCoords(&finalTexCoords[0]);
}
OBJModel::~OBJModel(){}

void OBJModel::drawOBJ(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glm::vec3 tempVertex;
	glm::vec2 tempUV;
	glm::vec3 tempNorm;
	//for debugging
	int a = this->getVerSize();
	int b = this->getUVSize();
	int c = this->getNormSize();

	glLoadIdentity();
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	//replace?
	glBindTexture(GL_TEXTURE_2D, this->getTex());

	//glRotatef(-90.f,0,1,0);

	glTranslatef(this->getPosX(), this->getPosY(), this->getPosZ());

	glBegin(GL_TRIANGLES);

	
	for(int j = 0; j < this->getVerSize(); j++){
		tempVertex = this->getVertex(j);
		tempUV = this->getUV(j);
		tempNorm = this->getNormal(j);
		
		glNormal3f( tempNorm.x,tempNorm.y,tempNorm.z);
		
		glTexCoord2f(tempUV.x,tempUV.y);
		
		glVertex3f( tempVertex.x,tempVertex.y,tempVertex.z);
	}
	
	glEnd();
	glPopMatrix();
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