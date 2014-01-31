#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>


enum shaderType
{
	GLSL_VERTEX_SHADER,
	GLSL_FRAGMENT_SHADER,
	//other shader types
};

class GLSLShader
{
private:
	shaderType type;

	unsigned int handle;

public:
	//Constructors
	GLSLShader(void);

	//Destructor
	~GLSLShader(void);

	//getters
	inline shaderType GetType(void)		{ return type;	}
	inline unsigned int GetHandle(void)	{ return handle;}

	//remove from graphics memory
	void Release(void);

	//Create the file based on either a string or a file
	int CreateShaderFromFile(shaderType newType, const char *filePath);
	int CreateShaderFromString(shaderType newType, const char *string);
};

class GLSLProgram
{
private:
	unsigned int handle;
public:
	//Getters
	inline unsigned int GetHandle(void) { return handle; }

	//Constructors
	GLSLProgram(void);

	//Destructor
	~GLSLProgram(void);

	//Remove from graphics memory
	void Release(void);

	//Attach a shader to the program
	//returns 0 if not attached, 1 if attached
	int AttachShader(GLSLShader *shader);

	//Link the shader
	int LinkProgram(void);

	//
	int ValidateProgram(void);

	//Activate the program for use in application
	void Activate(void);

	//Deactivate
	static void Deactivate(void);

	//
	int GetUniformLocation(const char *uniformName);

	//
	static void UniformFloat(int location, const float value);

	void UniformMat4(int location, bool transpose, const glm::mat4 value);
	//void UniformVec3List(int location, unsigned int numValues, const glm::vec3 *valueList);
};