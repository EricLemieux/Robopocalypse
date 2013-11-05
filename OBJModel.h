#ifndef OBJMODEL_H
#define OBJMODEL_H

class OBJModel{
public:
	OBJModel();
	OBJModel(const char *path, const char *texPath);
	~OBJModel();
	
	//movement



	//getter setters
	glm::vec3 getVertex(int i);
	sf::Vector2f getUV(int i);
	glm::vec3 getNormal(int i);

	int getVerSize();
	int getUVSize();
	int getNormSize();

	void setPos(float xtemp, float ytemp, float ztemp);
	float getPosX();
	float getPosY();
	float getPosZ();

	GLfloat getTex();

private:
	float x,y,z;
	float xspeed,yspeed,zspeed;
	float orientation;
	float dt;
	GLuint texture;

	sf::Image texMap;
	//std::vector<std::vector<int>> out_faces;
	std::vector<std::vector<int>> out_faces;
	std::vector<glm::vec3> out_vertices;
	std::vector<sf::Vector2f> out_uvs;
	std::vector<glm::vec3> out_normals;
};


#endif