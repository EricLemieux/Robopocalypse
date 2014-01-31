#include "Engine.h"


OBJModel::OBJModel(){}

OBJModel::OBJModel(const char *path, const char *texPath){
	std::vector<unsigned int> vuiVertexIndex,vuiUVIndex, vuiNormalIndex;
	std::vector<glm::vec3> vv3temp_vertex;
	std::vector<glm::vec2> vv2temp_uvs;
	std::vector<glm::vec3> vv3temp_normals;

	sf::Image texMap;
	if(!texMap.loadFromFile(texPath))
	{
		//The image didn't load, close the program!
		std::cout<<"PANIC, COULDN'T LOAD TEXTURE I'M SO SORRY\n";
	}

	
	glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, texMap.getSize().x, texMap.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, texMap.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	
	//FILE *file = fopen(path, "r");
	std::ifstream file;
	file.open(path);

	//if(file == NULL){
	//	std::cout<<"FILE ERROR /n";
	//}
	if(!file.is_open()){
		std::cout<<"OBJECT LOADING ERROR /n";
	}

	char lineHeader;
	//read file, convert from string to vector
	//while(fscanf(file, "%s", lineHeader) != EOF){
	while(!file.eof()){

		file.get(lineHeader);

		if(lineHeader == '#' || lineHeader == '/'){
			file.ignore(256,'\n');
		}

		else if(lineHeader == 'c'){
		
		}

		else if(lineHeader == 'v'){
			char secondChar;
			file.get(secondChar);
			//vertices
			if(secondChar==' '){
				char input[256];
				glm::vec3 num;
		
				file>>input;
				num.x = atof(input);
				file>>input;
				num.y=atof(input);
				file>>input;
				num.z=atof(input);

				vv3temp_vertex.push_back(num);
			}
			//UV, texture
			else if(secondChar == 't'){
				char input[256];
				glm::vec2 num;

				file>>input;
				num.x=atof(input);
				file>>input;
				num.y=atof(input);

				vv2temp_uvs.push_back(num);
			}
			//normals
			else if(secondChar == 'n'){
				char input[256];
				glm::vec3 num;
		
				file>>input;
				num.x = atof(input);
				file>>input;
				num.y=atof(input);
				file>>input;
				num.z=atof(input);

				vv3temp_normals.push_back(num);
			}

			else if (secondChar == 'p'){
			}
		}
			//faces
		else if (lineHeader == 'f'){
			std::string sVertex1,sVertex2,Vertex3;
			unsigned int uiVertexIndex[3], uiUVIndex[3], uiNormalIndex[3];
				
			std::string input;
			//file>>input;

			for(int i = 0; i < 3; ++i){
				//if(i!= 0)
				file>>input;

				int n = input.find('/');
				int m = input.find('/',2);

				std::string vi = input.substr(0,n);
				std::string uvi = input.substr(n+1,m);
				std::string ni = input.substr(m+1);

				if((atoi(vi.c_str()) != 0)||(atoi(uvi.c_str()) != 0)||(atoi(ni.c_str()) != 0)){
					uiVertexIndex[i] = atoi(vi.c_str());
					uiUVIndex[i] = atoi(uvi.c_str());
					uiNormalIndex[i] = atoi(ni.c_str());
				}
			}

			vuiVertexIndex.push_back(uiVertexIndex[0]);
			vuiVertexIndex.push_back(uiVertexIndex[1]);
			vuiVertexIndex.push_back(uiVertexIndex[2]);
			vuiUVIndex.push_back(uiUVIndex[0]);
			vuiUVIndex.push_back(uiUVIndex[1]);
			vuiUVIndex.push_back(uiUVIndex[2]);
			vuiNormalIndex.push_back(uiNormalIndex[0]);
			vuiNormalIndex.push_back(uiNormalIndex[1]);
			vuiNormalIndex.push_back(uiNormalIndex[2]);
		}
		else if(lineHeader == 'm'){
		}
		else if(lineHeader == 'u'){
		}
		else if(lineHeader == 'o'){
		}
		else if(lineHeader == 'g'){
		}
		else if(lineHeader == 's'){
		}
		else {
			file.ignore(256,'\n');
		}
		file.ignore(256,'\n');
	}
		
	file.close();

	//changing vectors into set of glm::vec3 for opengl
	for(unsigned int i=0; i<vuiVertexIndex.size();i++){
		unsigned int vertexIndex = vuiVertexIndex[i];
		glm::vec3 vertex = vv3temp_vertex[vertexIndex-1];
		out_vertices.push_back(vertex);
	}
	for(unsigned int j=0;j<vuiUVIndex.size();j++){
		unsigned int uvIndex = vuiUVIndex[j];
		glm::vec2 uv = vv2temp_uvs[uvIndex-1];
		out_uvs.push_back(uv);
	}
	for(unsigned int k=0;k<vuiNormalIndex.size();k++){
		unsigned int normalIndex = vuiNormalIndex[k];
		glm::vec3 normal = vv3temp_normals[normalIndex-1];
		out_normals.push_back(normal);
	}
	
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


	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->getTex());

	glRotatef(-90.f,0,1,0);

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