#include "Game.h"

//////////
//CONSTRUCTORS
//////////
Game::Game()
{
	//Init glfw for widow handleing
	if (!glfwInit())
		exit(EXIT_FAILURE);

	//Set the default game state
	GAME_STATE = STATE_GAMEPLAY;

	//Once initalised the game is now running
	isRunning = true;
}

Game::~Game()
{
}

//////////
//INITIALISERS
//////////

//Initialises for gameplay
void Game::initGameplay(void)
{
	projectionMatrix = glm::perspective(90.0f, (float)1280 / (float)720, 0.1f, 1000.0f);

	passProgram = new GLSLProgram;
	int result = 1;
	GLSLShader passShader_V, passShader_F;
	result *= passShader_V.CreateShaderFromFile(GLSL_VERTEX_SHADER,		"Resources/Shaders/pass_v.glsl");
	result *= passShader_F.CreateShaderFromFile(GLSL_FRAGMENT_SHADER,	"Resources/Shaders/pass_f.glsl");
	result *= passProgram->AttachShader(&passShader_V);
	result *= passProgram->AttachShader(&passShader_F);
	result *= passProgram->LinkProgram();
	result *= passProgram->ValidateProgram();

	//get uniform variables
	handle_MVP = passProgram->GetUniformLocation("MVP");

	//Create a game object for player1
	OBJModel playerModel("Resources/Models/Ball.obj", "Resources/Textures/Ball.jpg");
	player1.AttachModel(playerModel.GetVBO());
}

//Open a glfw window with defined size
void Game::OpenWindow(int width, int height)
{
	//Create window
	gameWindow = glfwCreateWindow(width, height, "Robobopocalypse", NULL, NULL);

	//If window didnt open
	if (!gameWindow)
	{
		glfwTerminate();
		std::cout << "Error opening GLFW window\n";
	}

	//Set the window as the current one
	glfwMakeContextCurrent(gameWindow);
}

//////////
//SETTERS
//////////



//////////
//GETTERS
//////////


//////////
//OTHERS
//////////

//Update the game
void Game::Update(void)
{
	//If the window should close, user presses the exit button, the game is no longer running and shutsdow at the start of gameloop
	if (glfwWindowShouldClose(gameWindow))
		isRunning = false;

	glfwPollEvents();
}

//Render the game
void Game::Render(void)
{
	//Clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Update the view matrix
	viewMatrix = glm::lookAt(glm::vec3(0, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	//Activate the shader and render the player
	passProgram->Activate();
	modelViewProjectionMatrix = player1.UpdateModelViewProjection(projectionMatrix, viewMatrix);
	glUniformMatrix4fv(handle_MVP, 1, 0, glm::value_ptr(modelViewProjectionMatrix));
	player1.Render();
	passProgram->Deactivate();

	//Swap front and back buffers
	glfwSwapBuffers(gameWindow);
}