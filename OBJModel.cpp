#include "Engine.h"


OBJModel::OBJModel(){}

OBJModel::OBJModel(const char *path, const char *texPath){
	std::vector<unsigned int> vuiVertexIndex,vuiUVIndex, vuiNormalIndex;
	std::vector<sf::Vector3f> vv3temp_vertex;
	std::vector<sf::Vector2f> vv2temp_uvs;
	std::vector<sf::Vector3f> vv3temp_normals;

	
	FILE *file = fopen(path, "r");
	//std::ifstream file;
	//file.open(path);
	if(file == NULL){
		std::cout<<"FILE ERROR /n";
	}
	char lineHeader[128];
	//read file, convert from string to vector
	while(fscanf(file, "%s", lineHeader) != EOF){
		//first word of line
		
		//std::string line;
		//std::string word;

		//getline(file,line);
		//break if eof
		//if(file.eof())
			

		//add vertices
		if(strcmp(lineHeader,"v")==0){
			sf::Vector3f vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vv3temp_vertex.push_back(vertex);
		} 
		// add texture vertices
		else if(strcmp(lineHeader, "vt")==0){
			sf::Vector2f uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = 1-uv.y;
			vv2temp_uvs.push_back(uv);
		}
		//add normal vertices
		else if(strcmp(lineHeader,"vn")==0){
			sf::Vector3f normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			vv3temp_normals.push_back(normal);
		}
		//add face indices: vertex, uv, normal
		else if(strcmp(lineHeader,"f")==0){
			std::string sVertex1,sVertex2,Vertex3;
			unsigned int uiVertexIndex[3], uiUVIndex[3], uiNormalIndex[3];
			int matches = fscanf(file,"%d/%d/%d %d/%d/%d %d/%d/%d\n",
									&uiVertexIndex[0],&uiUVIndex[0],&uiNormalIndex[0], 
									&uiVertexIndex[1],&uiUVIndex[1],&uiNormalIndex[1], 
									&uiVertexIndex[2],&uiUVIndex[2],&uiNormalIndex[2]);
			if(matches != 9){
				std::cout<<"File can't be read";
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
	}

	//changing vectors into set of flm::vec3 for opengl
	for(unsigned int i=0; i<vuiVertexIndex.size();i++){
		unsigned int vertexIndex = vuiVertexIndex[i];
		sf::Vector3f vertex = vv3temp_vertex[vertexIndex-1];
		out_vertices.push_back(vertex);
	}
	for(unsigned int j=0;j<vuiUVIndex.size();j++){
		unsigned int uvIndex = vuiUVIndex[j];
		sf::Vector2f uv = vv2temp_uvs[uvIndex-1];
		out_uvs.push_back(uv);
	}
	for(unsigned int k=0;k<vuiNormalIndex.size();k++){
		unsigned int normalIndex = vuiNormalIndex[k];
		sf::Vector3f normal = vv3temp_normals[normalIndex-1];
		out_normals.push_back(normal);
	}
	
}
OBJModel::~OBJModel(){}


sf::Vector3f OBJModel::getVertex(int i){
	return out_vertices[i];
}

sf::Vector2f OBJModel::getUV(int i){
	return out_uvs[i];
}

sf::Vector3f OBJModel::getNormal(int i){
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