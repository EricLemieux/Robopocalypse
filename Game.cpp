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
	
	world.AttachNode(&sceneGraph);

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

	lightProgram = new GLSLProgram;
	int result = 1;
	GLSLShader lightShader_V, lightShader_F;
	result *= lightShader_V.CreateShaderFromFile(GLSL_VERTEX_SHADER,	"Resources/Shaders/FragLighting_v.glsl");
	result *= lightShader_F.CreateShaderFromFile(GLSL_FRAGMENT_SHADER,	"Resources/Shaders/FragLighting_f.glsl");
	result *= lightProgram->AttachShader(&lightShader_V);
	result *= lightProgram->AttachShader(&lightShader_F);
	result *= lightProgram->LinkProgram();
	result *= lightProgram->ValidateProgram();

	//get uniform variables
	uniform_MVP			= lightProgram->GetUniformLocation("MVP");
	uniform_LightPos	= lightProgram->GetUniformLocation("lightPos");
	uniform_texture		= lightProgram->GetUniformLocation("objectTexture");

	//Create a game object for player1
	player1 = new GameObject;
	player1->AttachModel(OBJModel("Resources/Models/Robot.obj").GetVBO());
	player1->AttachTexture(&Texture("Resources/Textures/Shputnik_Texture.png"));
	sceneGraph.AttachNode(player1->GetNode());
	player1->SetPosition(glm::vec3(20, 0, -15));

	//Create a game object for player2
	player2 = new GameObject;
	player2->AttachModel(OBJModel("Resources/Models/Robot.obj").GetVBO());
	player2->AttachTexture(&Texture("Resources/Textures/Shputnik_Texture.png"));
	sceneGraph.AttachNode(player2->GetNode());
	player2->SetPosition(glm::vec3(-20, 0, -15));

	//Load the background objects into a asset list
	BackgroundObjects.Load("Resources/assets.txt");
	BackgroundObjects.AttachAllObjectsToNode(&sceneGraph);
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

	sceneGraph.Update();

	glfwPollEvents();
}

//Render the game
void Game::Render(void)
{
	//Clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Update the view matrix
	viewMatrix = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	//Activate the shader and render the player
	lightProgram->Activate();
	{
		//Render all of the background objects
		for (unsigned int i = 0;i < BackgroundObjects.GetSize(); ++i)
		{
			PreRender(BackgroundObjects.GetObjectAtIndex(i));
		}

		PreRender(player1);
		PreRender(player2);
	}
	lightProgram->Deactivate();

	//Swap front and back buffers
	glfwSwapBuffers(gameWindow);
}

void Game::PreRender(GameObject* object)
{
	modelViewProjectionMatrix = object->UpdateModelViewProjection(projectionMatrix, viewMatrix);
	glUniformMatrix4fv(uniform_MVP, 1, 0, glm::value_ptr(modelViewProjectionMatrix));
	glUniform3fv(uniform_LightPos, 1, glm::value_ptr(glm::inverse(modelViewProjectionMatrix) * glm::vec4(0, 0, 0, 1)));

	//glActiveTexture(GL_TEXTURE0);	
	glUniform1i(uniform_texture, 0);
	glBindTexture(GL_TEXTURE_2D, object->GetTexture()->GetHandle());
	
	object->Render();
}