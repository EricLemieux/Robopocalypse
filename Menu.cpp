#include "Menu.h"


Menu::Menu()
{
	glDeleteTextures(1, &textureHandle);

	program = new GLSLProgram;
	int result = 1;
	GLSLShader passShader_V, passShader_F;
	result *= passShader_V.CreateShaderFromFile(GLSL_VERTEX_SHADER,		"Resources/Shaders/pass_v.glsl");
	result *= passShader_F.CreateShaderFromFile(GLSL_FRAGMENT_SHADER,	"Resources/Shaders/HUD_f.glsl");
	result *= program->AttachShader(&passShader_V);
	result *= program->AttachShader(&passShader_F);
	result *= program->LinkProgram();
	result *= program->ValidateProgram();

	uniform_MVP				= program->GetUniformLocation("MVP");
	uniform_texture			= program->GetUniformLocation("objectTexture");
	uniform_flipDirection	= program->GetUniformLocation("flipDirection");

	FSQ = ShapeFullScreenQuad();
}


Menu::~Menu()
{
}

void Menu::Update(Game* game)
{
	glfwPollEvents();

	MenuInput(game);

	if (glfwWindowShouldClose(window))
		exit(123);
}

void Menu::Render(void)
{
	//Clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program->Activate();

	glUniformMatrix4fv(uniform_MVP, 1, 0, glm::value_ptr(glm::mat4(1)));

	glUniform2fv(uniform_flipDirection, 1, glm::value_ptr(glm::vec2(1,1)));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glUniform1i(uniform_texture, 0);

	glDisable(GL_CULL_FACE);

	//Render the Full Screen Quad
	FSQ->ActivateAndRender();

	program->Deactivate();

	//Swap front and back buffers
	glfwSwapBuffers(window);
}

void Menu::AttachBackground(char *filePath)
{
	textureHandle = ilutGLLoadImage(filePath);
	int a = 0;
}

void Menu::AttachWindow(GLFWwindow *gameWindow)
{
	window = gameWindow;
}

void Menu::MenuInput(Game* game)
{
	//Mouse position
	double * xpos = new double;
	double * ypos = new double;
	glfwGetCursorPos(window, xpos, ypos);

	//If pressed right mouse button
	int pressed = glfwGetMouseButton(window, 0);

	//TODO, if mouse pressed over buttons, STATE change if game start hit, exit hit
	// Hard coded :(  for 1600:800 resolution
	if ((*ypos <= 696.0) && (*ypos >= 494.0))
	{
		if ((*xpos >= 96.0) && (*xpos <= 298.0))
		{
			if (pressed == 1)
			{
				game->SetState(STATE_GAMEPLAY);
				if (game->hasBeenInit)
					game->resetGameplay();
				else
					game->initGameplay();
			}
		}
		else if ((*xpos >= 1298.0) && (*xpos <= 1500.0))
		{
			if (pressed == 1)
			{
				exit(99);
			}
		}
	}

	if (glfwJoystickPresent(0) == GL_TRUE)
	{
		//Get joystick input
		int * joySizeP1;
		joySizeP1 = new int;
		int * buttonSizeP1;
		buttonSizeP1 = new int;
		const unsigned char* buttonPointerP1 = glfwGetJoystickButtons(0, buttonSizeP1);

		////joystick player 1
		//for (unsigned int i = 0; i < 14; ++i)
		//{
		//	if (*(buttonPointerP1 + i) == 1)
		//	{
		//		std::cout << i << "\n";
		//		std::cout << "moo\n";
		//	}
		//}

		if ((*(buttonPointerP1 + 0) == 1) || (*(buttonPointerP1 + 7) == 1))
		{
			game->SetState(STATE_GAMEPLAY);
			if (game->hasBeenInit)
				game->resetGameplay();
			else
				game->initGameplay();
		}
		else if (*(buttonPointerP1 + 1) == 1){
			exit(99);
		}
		free(joySizeP1);
		free(buttonSizeP1);
	}

	if (glfwJoystickPresent(1) == GL_TRUE)
	{
		//Get joystick input
		int * joySizeP2;
		joySizeP2 = new int;
		int * buttonSizeP2;
		buttonSizeP2 = new int;
		const unsigned char* buttonPointerP2 = glfwGetJoystickButtons(1, buttonSizeP2);

		//joystick player 2
		if ((*(buttonPointerP2 + 0) == 1) || (*(buttonPointerP2 + 7) == 1))
		{
			game->SetState(STATE_GAMEPLAY);
			if (game->hasBeenInit)
				game->resetGameplay();
			else
				game->initGameplay();
		}
		else if (*(buttonPointerP2 + 1) == 1){
			exit(99);
		}
		free(joySizeP2);
		free(buttonSizeP2);
	}
}