#pragma once

#include <glm\glm.hpp>

class VertexBuffer
{
private:
	//handles
	unsigned int vertexHandle;
	unsigned int normalHandle;
	unsigned int texCoordHandle;
	unsigned int vaoHandle;

	//number of verticies in the model
	unsigned int numberOfVerticies;

public:
	//Constructor
	VertexBuffer(void);

	//Destructor
	~VertexBuffer(void);

	//Release from graphics memory
	void Release(void);

	//init VAO
	int Initialize(unsigned int numVerticies, bool useNormals, bool useTexCoords);

	//Add attribute data
	int AddVerticies(float *rawVertices);
	int AddNormals(float *rawNormals);
	int AddTexCoords(float *rawTexCoords);

	//Activate VAO for rendering
	void Activate(void);

	//Deactivate VAO from rendering
	void Deactivate(void);

	//Render this VAO
	void Render(void);
	void ActivateAndRender(void);
};