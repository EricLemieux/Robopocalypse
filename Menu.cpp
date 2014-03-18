#include "Menu.h"


Menu::Menu()
{
	glDeleteTextures(1, &textureHandle);

	program = new GLSLProgram;
	int result = 1;
	GLSLShader passShader_V, passShader_F;
	result *= passShader_V.CreateShaderFromFile(GLSL_VERTEX_SHADER,		"Resources/Shaders/pass_v.glsl");
	result *= passShader_F.CreateShaderFromFile(GLSL_FRAGMENT_SHADER,	"Resources/Shaders/pass_f.glsl");
	result *= program->AttachShader(&passShader_V);
	result *= program->AttachShader(&passShader_F);
	result *= program->LinkProgram();
	result *= program->ValidateProgram();

	uniform_MVP		= program->GetUniformLocation("MVP");
	uniform_texture = program->GetUniformLocation("objectTexture");

	FSQ = ShapeFullScreenQuad();
}


Menu::~Menu()
{
}

void Menu::Update(void)
{
	glfwPollEvents();

	MenuInput();

	if (glfwWindowShouldClose(window))
		exit(123);
}

void Menu::Render(void)
{
	//Clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program->Activate();

	glUniformMatrix4fv(uniform_MVP, 1, 0, glm::value_ptr(glm::mat4(1)));

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
}

void Menu::AttachWindow(GLFWwindow *gameWindow)
{
	window = gameWindow;
}

void Menu::MenuInput(void)
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
			if (pressed == 1){}
			//WORKS change gamestate to gameplay
		}
		else if ((*xpos >= 1298.0) && (*xpos <= 1500.0))
		{
			if (pressed == 1){}
			//WORKS exit game
		}
	}
}