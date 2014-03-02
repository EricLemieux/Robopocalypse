#pragma once

#include <iostream>

//#include <GL\glew.h>

#undef _UNICODE
#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>

inline GLuint loadTexture(char *filePath)
{
	return ilutGLLoadImage(filePath);
}