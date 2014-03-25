#pragma once

#include "GLSL.h"
#include "VertexBuffer.h"
#include "BasicShapes.h"

#undef _UNICODE
#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>

#include <GLFW\glfw3.h>

#include "Game.h"

//Predefine for Cyclic Dependency
class Game;

class Menu
{
public:
	Menu();
	~Menu();

	void Render(void);
	void Update(Game* game);

	void AttachBackground(char *filePath);

	void AttachWindow(GLFWwindow *gameWindow);

	void Menu::MenuInput(Game* game);

private:
	GLuint textureHandle;

	unsigned int uniform_texture;
	unsigned int uniform_MVP;
	unsigned int uniform_flipDirection;

	GLSLProgram *program;

	GLFWwindow *window;

	VertexBuffer *FSQ;
};