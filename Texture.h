#pragma once

#include <iostream>

//#include <GL\glew.h>

#undef _UNICODE
#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>

class Texture
{
public:
	Texture();
	Texture(char* filePath);
	~Texture();

	int Load(char* filePath);

	inline GLuint* GetHandle(void){ return &handle; }

private:
	GLuint handle;
};