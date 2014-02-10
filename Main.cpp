//////////
//INCLUDES
//////////

//Graphics
#include <GL\glew.h>

//Window and input handleing
//#include <GLFW\glfw3.h>

//Math
#include <glm\glm.hpp>
#include <glm\ext.hpp>

//Image handling
#undef _UNICODE
#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>

//Game
#include "Game.h"

void initOpenGL(void);

int main()
{
	//Init the game
	Game *Robopocalypse = new Game;

	//Open a window for the game
	Robopocalypse->openWindow(1280, 720);

	//Init GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
		std::cout << "glewInit() Error\n";
	
	//Init OpenGL
	initOpenGL();

	//While the window is open run the game
	while (Robopocalypse->getIfRunning())
	{
		if (Robopocalypse->getState() == STATE_GAMEPLAY)
		{
			Robopocalypse->update();
			Robopocalypse->render();
		}
		else if (Robopocalypse->getState() == STATE_MAINMENU)
		{

		}
	}
	
	//Shutdown gldw
	glfwTerminate();

	//End the program
	return 0;
}

//
void initOpenGL(void)
{
	glClearColor(0.3f, 0.3f, 0.9f, 1.0f);
}