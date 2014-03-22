#include <GL\glew.h>
#include "VertexBuffer.h"

//Constructor
VertexBuffer::VertexBuffer(void)
{
	vertexHandle = 0;
	normalHandle = 0;
	texCoordHandle = 0;

	vaoHandle = 0;

	numberOfVerticies = 0;
}

//Destructor
VertexBuffer::~VertexBuffer(void)
{
	Release();
}

//Release from graphics memory
void VertexBuffer::Release(void)
{
	if (vaoHandle)
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &vaoHandle);

		glDeleteBuffers(1, &vertexHandle);
		if (normalHandle)
		{
			glDeleteBuffers(1, &normalHandle);
		}
		if (texCoordHandle)
		{
			glDeleteBuffers(1, &texCoordHandle);
		}

		vertexHandle = normalHandle = texCoordHandle = vaoHandle = numberOfVerticies = 0;
	}
}

//init VAO
int VertexBuffer::Initialize(unsigned int numVertices, bool useNormals, bool useTexCoords, bool useBones)
{
	usingTexcoords	= useTexCoords;
	usingNormals	= useNormals;
	usingBones		= useBones;

	if (!vaoHandle)
	{
		numberOfVerticies = numVertices;

		glGenVertexArrays(1, &vaoHandle);
		glBindVertexArray(vaoHandle);

		glGenBuffers(1, &vertexHandle);
		glBindBuffer(GL_ARRAY_BUFFER, vertexHandle);

		const unsigned int vertexBufferSize = numVertices * sizeof(float)* 3;

		glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, 0, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		if (useNormals)
		{
			glGenBuffers(1, &normalHandle);
			glBindBuffer(GL_ARRAY_BUFFER, normalHandle);

			const unsigned int normalBufferSize = numVertices * sizeof(float)* 3;

			glBufferData(GL_ARRAY_BUFFER, normalBufferSize, 0, GL_STATIC_DRAW);

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (useTexCoords)
		{
			glGenBuffers(1, &texCoordHandle);
			glBindBuffer(GL_ARRAY_BUFFER, texCoordHandle);

			const unsigned int texCoordBufferSize = numVertices * sizeof(float)* 2;

			glBufferData(GL_ARRAY_BUFFER, texCoordBufferSize, 0, GL_STATIC_DRAW);

			glEnableVertexAttribArray(8);
			glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (useBones)
		{
			//Generate the bone index handle
			glGenBuffers(1, &boneIndexHandle);
			glBindBuffer(GL_ARRAY_BUFFER, boneIndexHandle);

			const unsigned int boneIndexBufferSize = numVertices * sizeof(float) * 4;

			glBufferData(GL_ARRAY_BUFFER, boneIndexBufferSize, 0, GL_STATIC_DRAW);

			glEnableVertexAttribArray(10);
			glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, 0, 0);

			//Generate the bone weights handle
			glGenBuffers(1, &boneWeightHandle);
			glBindBuffer(GL_ARRAY_BUFFER, boneWeightHandle);

			const unsigned int bonewWeightBufferSize = numVertices * sizeof(float) * 4;

			glBufferData(GL_ARRAY_BUFFER, bonewWeightBufferSize, 0, GL_STATIC_DRAW);

			glEnableVertexAttribArray(11);
			glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, 0, 0);
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return 1;
	}
	return 0;
}

//Add attribute data
int VertexBuffer::AddVerticies(float *rawVertices)
{
	if (vertexHandle && rawVertices)
	{
		const unsigned int bufferSize = numberOfVerticies * sizeof(float)* 3;
		glBindBuffer(GL_ARRAY_BUFFER, vertexHandle);
		glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, rawVertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return 1;
	}
	return 0;
}
int VertexBuffer::AddNormals(float *rawNormals)
{
	if (normalHandle && rawNormals)
	{
		const unsigned int bufferSize = numberOfVerticies * sizeof(float)* 3;
		glBindBuffer(GL_ARRAY_BUFFER, normalHandle);
		glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, rawNormals);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return 1;
	}
	return 0;
}
int VertexBuffer::AddTexCoords(float *rawTexCoords)
{
	if (texCoordHandle && rawTexCoords)
	{
		const unsigned int bufferSize = numberOfVerticies * sizeof(float)* 2;
		glBindBuffer(GL_ARRAY_BUFFER, texCoordHandle);
		glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, rawTexCoords);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return 1;
	}
	return 0;
}

int VertexBuffer::AddBoneIndexes(float *rawBoneIndexes)
{
	if (boneIndexHandle && rawBoneIndexes)
	{
		const unsigned int bufferSize = numberOfVerticies * sizeof(float)* 4;
		glBindBuffer(GL_ARRAY_BUFFER, boneIndexHandle);
		glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, rawBoneIndexes);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return 1;
	}
	return 0;
}
int VertexBuffer::AddBoneWeights(float *rawBoneWeights)
{
	if (boneWeightHandle && rawBoneWeights)
	{
		const unsigned int bufferSize = numberOfVerticies * sizeof(float)* 4;
		glBindBuffer(GL_ARRAY_BUFFER, boneWeightHandle);
		glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, rawBoneWeights);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return 1;
	}
	return 0;
}

//Activate VAO for rendering
void VertexBuffer::Activate(void)
{
	glBindVertexArray(vaoHandle);
}

//Deactivate VAO from rendering
void VertexBuffer::Deactivate(void)
{
	//Set to NULL
	glBindVertexArray(0);
}

//Render this VAO
void VertexBuffer::Render(void)
{
	//if this vao exists in memory
	if (vaoHandle)
	{
		GLenum error = glGetError();

		//Render it
		glDrawArrays(GL_TRIANGLES, 0, numberOfVerticies);

		int a = 0;
	}
}
void VertexBuffer::ActivateAndRender(void)
{
	//Call both activate and render
	//the lazy way...
	Activate();
	Render();
}