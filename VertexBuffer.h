#pragma once

#include <iostream>

#include <glm\glm.hpp>

class VertexBuffer
{
private:
	//handles
	unsigned int vertexHandle;
	unsigned int normalHandle;
	unsigned int texCoordHandle;
	unsigned int vaoHandle;
	unsigned int boneIndexHandle;
	unsigned int boneWeightHandle;

	//number of verticies in the model
	unsigned int numberOfVerticies;

	bool usingTexcoords, usingNormals, usingBones;

public:
	//Constructor
	VertexBuffer(void);

	//Destructor
	~VertexBuffer(void);

	//Release from graphics memory
	void Release(void);

	//init VAO
	int Initialize(unsigned int numVerticies, bool useNormals, bool useTexCoords, bool useBones = false);

	//Add attribute data
	int AddVerticies(float *rawVertices);
	int AddNormals(float *rawNormals);
	int AddTexCoords(float *rawTexCoords);

	int AddBoneIndexes(float *rawBoneIndexes);
	int AddBoneWeights(float *rawBoneWeights);

	//Activate VAO for rendering
	void Activate(void);

	//Deactivate VAO from rendering
	static void Deactivate(void);

	//Render this VAO
	void Render(void);
	void ActivateAndRender(void);

	//Get the amount of verticies in the VBO
	inline unsigned int GetNumVerticies(void){ return numberOfVerticies; }

	//Get whether the VBO is using atributes such as textures and normals
	inline bool UsingTexcoords(void){return usingTexcoords;}
	inline bool Usingnormals(void)	{return usingNormals;}
	inline bool UsingBones(void)	{return usingBones;}
};