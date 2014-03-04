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