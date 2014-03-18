#pragma once

#include "GLSL.h"
#include "VertexBuffer.h"
#include "BasicShapes.h"

#undef _UNICODE
#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>

#include <GLFW\glfw3.h>

class Menu
{
public:
	Menu();
	~Menu();

	void Render(void);
	void Update(void);

	void AttachBackground(char *filePath);

	void AttachWindow(GLFWwindow *gameWindow);

	void Menu::MenuInput(void);

private:
	GLuint textureHandle;

	unsigned int uniform_texture;
	unsigned int uniform_MVP;

	GLSLProgram *program;

	GLFWwindow *window;

	VertexBuffer *FSQ;
};