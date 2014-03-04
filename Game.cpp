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
	GAME_STATE = STATE_MAINMENU;

	world = new Node;
	sceneGraph = new Node;
	
	world->AttachNode(sceneGraph);

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
	GAME_STATE = STATE_GAMEPLAY;

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
	uniform_normalMap	= lightProgram->GetUniformLocation("objectNormalMap");

	//Create the game object for the main camera
	mainCamera = new Camera;
	mainCamera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
	sceneGraph->AttachNode(mainCamera->GetNode());
	
	//Create a game object for player1
	player1 = new Player;
	player1->AttachModel(OBJModel("Resources/Models/Robot.obj").GetVBO());
	player1->AttachTexture(loadTexture("Resources/Textures/Shputnik_Texture_red.png"));
	player1->SetPosition(glm::vec3(20, 0, -15));
	sceneGraph->AttachNode(player1->GetNode());
	
	//Create a game object for player2
	player2 = new Player;
	player2->AttachModel(OBJModel("Resources/Models/Robot.obj").GetVBO());
	player2->AttachTexture(loadTexture("Resources/Textures/Shputnik_Texture_blue.png"));
	player2->AttachNormalMap(loadTexture("Resources/NormalMaps/testMap.jpg"));
	player2->SetPosition(glm::vec3(-20, 0, -15));
	sceneGraph->AttachNode(player2->GetNode());
	
	//Load the background objects into a asset list
	BackgroundObjects.Load("Resources/assets.txt");
	BackgroundObjects.AttachAllObjectsToNode(sceneGraph);

	sceneGraph->Update();
}

//Initialises for menu
void Game::initMainMenu(void)
{
	GAME_STATE = STATE_MAINMENU;

	gameMenu = new Menu;
	gameMenu->AttachBackground("Resources/Textures/MainMenu.png");
	gameMenu->AttachWindow(gameWindow);
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
	glfwPollEvents();

	//If the window should close, user presses the exit button, the game is no longer running and shutsdown at the start of gameloop
	if (glfwWindowShouldClose(gameWindow))
		isRunning = false;

	playerInput();
	
	player1->update(*player2);
	player2->update(*player1);

	sceneGraph->Update();
}

void Game::playerInput(void){
	//player 1 input
	if (glfwGetKey(gameWindow, 'S') == GLFW_PRESS){
		player1->controllerInput(MOVE_LEFT);
	}
	else if (glfwGetKey(gameWindow, 'F') == GLFW_PRESS){
		player1->controllerInput(MOVE_RIGHT);
	}
	else if (glfwGetKey(gameWindow, 'E') == GLFW_PRESS){
		player1->controllerInput(JUMP);
	}
	else if (glfwGetKey(gameWindow, 'D') == GLFW_PRESS){
		player1->controllerInput(BLOCK);
	}
	else if (glfwGetKey(gameWindow, 'W') == GLFW_PRESS){
		player1->controllerInput(DASH_LEFT);
	}
	else if (glfwGetKey(gameWindow, 'R') == GLFW_PRESS){
		player1->controllerInput(DASH_RIGHT);
	}
	else if (glfwGetKey(gameWindow, 'Q') == GLFW_PRESS){
		player1->controllerInput(PUNCH);
	}
	else if (glfwGetKey(gameWindow, 'A') == GLFW_PRESS){
		player1->controllerInput(KICK);
	}
	else if (glfwGetKey(gameWindow, 'T') == GLFW_PRESS){
		player1->controllerInput(LASER);
	}
	else if (glfwGetKey(gameWindow, 'G') == GLFW_PRESS){
		player1->controllerInput(BLAST);
	}

	//player 2 input
	if (glfwGetKey(gameWindow, 'J') == GLFW_PRESS){
		player2->controllerInput(MOVE_LEFT);
	}
	else if (glfwGetKey(gameWindow, 'L') == GLFW_PRESS){
		player2->controllerInput(MOVE_RIGHT);
	}
	else if (glfwGetKey(gameWindow, 'I') == GLFW_PRESS){
		player2->controllerInput(JUMP);
	}
	else if (glfwGetKey(gameWindow, 'K') == GLFW_PRESS){
		player2->controllerInput(BLOCK);
	}
	else if (glfwGetKey(gameWindow, 'U') == GLFW_PRESS){
		player2->controllerInput(DASH_LEFT);
	}
	else if (glfwGetKey(gameWindow, 'O') == GLFW_PRESS){
		player2->controllerInput(DASH_RIGHT);
	}
	else if (glfwGetKey(gameWindow, 'Y') == GLFW_PRESS){
		player2->controllerInput(LASER);
	}
	else if (glfwGetKey(gameWindow, 'H') == GLFW_PRESS){
		player2->controllerInput(BLAST);
	}
	else if (glfwGetKey(gameWindow, 'P') == GLFW_PRESS){
		player2->controllerInput(PUNCH);
	}
	else if (glfwGetKey(gameWindow, ';') == GLFW_PRESS){
		player2->controllerInput(KICK);
	}
}


//Render the game
void Game::Render(void)
{
	//Clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Update the view matrix
	mainCamera->Update(&viewMatrix);

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

		PreRender(player1->GetCollisionBoxes());
		PreRender(player2->GetCollisionBoxes());
	}
	lightProgram->Deactivate();

	//Swap front and back buffers
	glfwSwapBuffers(gameWindow);
}

void Game::PreRender(GameObject* object)
{
	modelViewProjectionMatrix = object->UpdateModelViewProjection(projectionMatrix, viewMatrix);
	glUniformMatrix4fv(uniform_MVP, 1, 0, glm::value_ptr(modelViewProjectionMatrix));
	glUniform3fv(uniform_LightPos, 1, glm::value_ptr(glm::inverse(modelViewProjectionMatrix) * glm::vec4(0.0f, 0.0f, -0.5f, 0.0f)));
	
	//Pass in texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, object->GetTextureHandle());
	glUniform1i(uniform_texture, 0);
	
	//Pass in normal map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, object->GetNormalMapHandle());
	glUniform1i(uniform_normalMap, 1);

	object->Render();
}

//This draws a vector of hitboxes, only used for debugging
void Game::PreRender(std::vector<CollisionBox> hitboxes)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (unsigned int i = 0; i < hitboxes.size(); ++i)
	{
		modelViewProjectionMatrix = projectionMatrix * viewMatrix * hitboxes[i].GetSceneGraphObject()->GetWorldTransform();
		glUniformMatrix4fv(uniform_MVP, 1, 0, glm::value_ptr(modelViewProjectionMatrix));
		hitboxes[i].GetVertexBuffer()->ActivateAndRender();
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}