//////////
//INCLUDES
//////////

//Graphics
#include <GL\glew.h>

//Window and input handling
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

static const double FPS_REFRESH_CAP = 1.0f / 60.0f;

int main()
{
	//Init the game
	Game *Robopocalypse = new Game;

	//Open a window for the game
	Robopocalypse->OpenWindow(1280, 720);

	//Init GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
		std::cout << "glewInit() Error\n";

	//Init DevIL
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	
	//Init OpenGL
	initOpenGL();

	//Load a blank image into spot #1 using this for objects that dont have a normal map, there is probably a way better way of doing this...
	loadTexture("Resources/NormalMaps/blank.png");

	Robopocalypse->initGameplay();
	//Robopocalypse->initMainMenu();

	//Set the time to zero before running the game loop so we know it starts from scratch.
	glfwSetTime(0.0f);

	float deltaTime		= 0.0f;
	double timeOfLastUpdate	= 0.0f;

	//While the window is open run the game
	while (Robopocalypse->GetIfRunning())
	{
		//Update the deltaTime
		deltaTime = glfwGetTime() - timeOfLastUpdate;

		//Makes sure there has been enough time between updates before rendering to the screen.
		if (deltaTime >= FPS_REFRESH_CAP)
		{
			if (Robopocalypse->GetState() == STATE_GAMEPLAY)
			{
				Robopocalypse->Update();
				Robopocalypse->Render();
			}
			else if (Robopocalypse->GetState() == STATE_MAINMENU)
			{
				Robopocalypse->GetMenu()->Update(Robopocalypse);
				Robopocalypse->GetMenu()->Render();
			}

			//Reset the time between updates
			deltaTime			= 0.0f;
			timeOfLastUpdate	= glfwGetTime();
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
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(20.f);
}