#include "Game.h"

//////////
//CONSTRUCTORS
//////////
Game::Game()
{
	//Init glfw for widow handling
	if (!glfwInit())
		exit(EXIT_FAILURE);

	//Set the default game state
	GAME_STATE = STATE_MAINMENU;

	world = new Node;
	sceneGraph = new Node;
	
	world->AttachNode(sceneGraph);

	//Once initialized the game is now running
	isRunning = true;
	
	//load sounds
	soundSystem.loadSound("Resources/sound_assets.txt");
}

Game::~Game()
{
}

//////////
//INITIALISERS
//////////

//Initializes for gameplay
void Game::initGameplay(void)
{
	GAME_STATE = STATE_GAMEPLAY;

	projectionMatrix = glm::perspective(90.0f, (float)1280 / (float)720, 0.1f, 1000.0f);
	
	//Create the full screen quad
	fullScreenQuad = ShapeFullScreenQuad();

	diffuseProgram = new GLSLProgram;
	int result = 1;
	GLSLShader diffuseShader_V, diffuseShader_G, diffuseShader_F;
	result *= diffuseShader_V.CreateShaderFromFile(GLSL_VERTEX_SHADER,	"Resources/Shaders/MeshSkinning_v.glsl");	//CHANGE BACK TO 'pass_v.glsl'
	result *= diffuseShader_G.CreateShaderFromFile(GLSL_GEOMETRY_SHADER,"Resources/Shaders/pass_g.glsl");
	result *= diffuseShader_F.CreateShaderFromFile(GLSL_FRAGMENT_SHADER,"Resources/Shaders/Diffuse_f.glsl");
	result *= diffuseProgram->AttachShader(&diffuseShader_V);
	result *= diffuseProgram->AttachShader(&diffuseShader_G);
	result *= diffuseProgram->AttachShader(&diffuseShader_F);
	result *= diffuseProgram->LinkProgram();
	result *= diffuseProgram->ValidateProgram();

	diffuseShader_F.Release();
	diffuseShader_G.Release();
	diffuseShader_V.Release();

	//get uniform variables
	uniform_MVP			= diffuseProgram->GetUniformLocation("MVP");
	uniform_texture		= diffuseProgram->GetUniformLocation("objectTexture");
	uniform_normalMap	= diffuseProgram->GetUniformLocation("objectNormalMap");

	//TEMP
	char *name;
	for (unsigned int i = 0; i < 31; ++i)
	{
		name = new char;
		sprintf(name, "boneMatricies[%i]", i);
		uniform_boneMat[i] = diffuseProgram->GetUniformLocation(name);
	}

	//Set up the HUD
	{
		HUD = new GameObject;

		//Set up the pass though shader
		HUDProgram = new GLSLProgram;
		int result = 1;
		GLSLShader HUDShader_V,HUDShader_G, HUDShader_F;
		result *= HUDShader_V.CreateShaderFromFile(GLSL_VERTEX_SHADER,	"Resources/Shaders/pass_v.glsl");
		result *= HUDShader_G.CreateShaderFromFile(GLSL_GEOMETRY_SHADER,"Resources/Shaders/pass_g.glsl");
		result *= HUDShader_F.CreateShaderFromFile(GLSL_FRAGMENT_SHADER,"Resources/Shaders/HUD_f.glsl");
		result *= HUDProgram->AttachShader(&HUDShader_V);
		result *= HUDProgram->AttachShader(&HUDShader_G);
		result *= HUDProgram->AttachShader(&HUDShader_F);
		result *= HUDProgram->LinkProgram();
		result *= HUDProgram->ValidateProgram();

		HUDShader_F.Release();
		HUDShader_G.Release();
		HUDShader_V.Release();

		//get uniform variables
		uniform_HUD_MVP				= HUDProgram->GetUniformLocation("MVP");
		uniform_HUD_texture			= HUDProgram->GetUniformLocation("objectTexture");
		uniform_HUD_FaceDirection	= HUDProgram->GetUniformLocation("flipDirection");

		//Load the HUD texture
		HUDBackgroundHandle = loadTexture("Resources/Textures/HUDback.png");
		HUDBarRedHandle		= loadTexture("Resources/Textures/RedBarSampler.png");
		HUDBarBlueHandle	= loadTexture("Resources/Textures/RedBarSampler.png");

		HUD->AttachModel(ShapeHUDQuad(1.0f,0.1f));
		HUD->GetNode()->TranslateNode(glm::vec3(0,0.9f,0));
		sceneGraph->AttachNode(HUD->GetNode());

		for (unsigned int i = 0; i < 4; ++i)
		{
			HUDBars[i] = new GameObject;

			if (i == 0 || i == 1)
				HUDBars[i]->AttachModel(ShapeHUDQuad(0.7f, 0.02f, 1.0f, 1));
			else
				HUDBars[i]->AttachModel(ShapeHUDQuad(0.7f, 0.02f, 1.0f, -1));
			
			HUD->GetNode()->AttachNode(HUDBars[i]->GetNode());

			//Translate the HUD bars around the screen
			//TODO: Need to find a better way of doing this
			float horizontal	= 0.15f;
			float vertical		= 0.046f;
			if (i == 0)
				HUDBars[i]->GetNode()->TranslateNode(glm::vec3(-horizontal, vertical, 0));
			else if (i == 1)
				HUDBars[i]->GetNode()->TranslateNode(glm::vec3(-horizontal, -vertical, 0));
			else if (i == 2)
				HUDBars[i]->GetNode()->TranslateNode(glm::vec3(horizontal, vertical, 0));
			else if (i == 3)
				HUDBars[i]->GetNode()->TranslateNode(glm::vec3(horizontal, -vertical, 0));
		}
	}

	//Create the game object for the main camera
	mainCamera = new Camera;
	mainCamera->SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
	sceneGraph->AttachNode(mainCamera->GetNode());
	
	//Create a game object for player1
	player1 = new Player;
	player1->AttachModel(OBJModel("Resources/Models/Robot.obj").GetVBO());
	player1->AttachBones("Resources/Bones/body.xml");
	player1->AttachTexture(loadTexture("Resources/Textures/Shputnik_Texture_red.png"));
	player1->SetPosition(glm::vec3(-17, 0, -15));
	player1->GetNode()->SetRotation(glm::vec3(90,0,0));
	sceneGraph->AttachNode(player1->GetNode());
	
	//Create a game object for player2
	player2 = new Player;
	player2->AttachModel(OBJModel("Resources/Models/Robot.obj").GetVBO());
	player2->AttachBones("Resources/Bones/body.xml");
	player2->AttachTexture(loadTexture("Resources/Textures/Shputnik_Texture_blue.png"));
	player2->AttachNormalMap(loadTexture("Resources/NormalMaps/testMap.jpg"));
	player2->SetPosition(glm::vec3(17, 0, -15));
	player2->GetNode()->SetRotation(glm::vec3(-90, 0, 0));
	sceneGraph->AttachNode(player2->GetNode());
	
	//Load the background objects into a asset list
	BackgroundObjects.Load("Resources/assets.txt");
	BackgroundObjects.AttachAllObjectsToNode(sceneGraph);

	//Setting up the outline shader for use with toon shading
	OutlineProgram = new GLSLProgram;
	result = 1;
	GLSLShader outlineShader_V, outlineShader_G, outlineShader_F;
	result *= outlineShader_V.CreateShaderFromFile(GLSL_VERTEX_SHADER,	"Resources/Shaders/pass_v.glsl");
	result *= outlineShader_G.CreateShaderFromFile(GLSL_GEOMETRY_SHADER,"Resources/Shaders/pass_g.glsl");
	result *= outlineShader_F.CreateShaderFromFile(GLSL_FRAGMENT_SHADER,"Resources/Shaders/outline_f.glsl");
	result *= OutlineProgram->AttachShader(&outlineShader_V);
	result *= OutlineProgram->AttachShader(&outlineShader_F);
	result *= OutlineProgram->LinkProgram();
	result *= OutlineProgram->ValidateProgram();

	outlineShader_F.Release();
	outlineShader_G.Release();
	outlineShader_V.Release();

	uniform_outline_MVP = OutlineProgram->GetUniformLocation("MVP");
	uniform_outline_scene = OutlineProgram->GetUniformLocation("scene");

	//Set up the first pass Frame buffer
	firstPass = new FrameBuffer;
	firstPass->Initialize(windowWidth, windowHeight, 2, true, false);

	mainLight = new Light;
	mainLight->SetColour(glm::vec3(1, 1, 1));
	mainLight->AttachModel(OBJModel("Resources/Models/Ball.obj").GetVBO());
	mainLight->SetPosition(glm::vec3(0.0f, 1.0f, 1.0f));
	mainLight->GetNode()->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	sceneGraph->AttachNode(mainLight->GetNode());

	qMap_handle = loadTexture("Resources/Textures/qMap.png");

	soundSystem.playSound(0, BGM_FIGHT_CHANNEL);

	sceneGraph->Update();
}

//Initializes for menu
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
	//Set the size of the window to be stored.
	windowWidth		= width;
	windowHeight	= height;

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
	
	player1->update(player2, pl1SFX);
	player2->update(player1, pl2SFX);

	soundSystem.setChannelPos(SFX_PLAYER1_CHANNEL, player1->getPos());
	soundSystem.setChannelPos(SFX_PLAYER2_CHANNEL, player2->getPos());

	if (pl1SFX != EMPTY_P_SFX)
		soundSystem.playSound(pl1SFX, SFX_PLAYER1_CHANNEL);

	if (pl2SFX != EMPTY_P_SFX)
		soundSystem.playSound(pl2SFX, SFX_PLAYER2_CHANNEL);

	mainCamera->SetTarget(glm::vec3((player1->getPos().x + player2->getPos().x) / 2.f, 0, 0));
	mainCamera->SetPosition(glm::vec3((player1->getPos().x + player2->getPos().x) / 2.f, 0, 5));

	HUDBars[0]->AttachModel(ShapeHUDQuad(0.7f, 0.02f, (float)player1->GetHP() / (float)player1->GetMaxHP(), 1));
	HUDBars[1]->AttachModel(ShapeHUDQuad(0.7f, 0.02f, (float)player1->GetSP() / (float)player1->GetMaxSP(), 1));
													  							
	HUDBars[2]->AttachModel(ShapeHUDQuad(0.7f, 0.02f, (float)player2->GetHP() / (float)player2->GetMaxHP(), -1));
	HUDBars[3]->AttachModel(ShapeHUDQuad(0.7f, 0.02f, (float)player2->GetSP() / (float)player2->GetMaxSP(), -1));

	soundSystem.updateSound();

	sceneGraph->Update();
}

void Game::playerInput(void){
	//joystick player 1
	if (glfwJoystickPresent(0) == GL_TRUE){

		//initialize player 1 inputs
		int * joySize;
		joySize = new int;
		const float* joystickPointer = glfwGetJoystickAxes(0, joySize);
		int * buttonSize;
		buttonSize = new int;
		const unsigned char* buttonPointer = glfwGetJoystickButtons(0, buttonSize);

		if ((*joystickPointer <= -0.25f) || (*(buttonPointer + 13) == 1)){
			player1->controllerInput(MOVE_LEFT);
		}
		else if ((*joystickPointer >= 0.25f) || (*(buttonPointer + 11) == 1)){
			player1->controllerInput(MOVE_RIGHT);
		}
		if ((*(joystickPointer + 1) <= -0.50f) || (*(buttonPointer + 10) == 1)){
			player1->controllerInput(BLOCK);
		}
		if (*(buttonPointer + 4) == 1){
			player1->controllerInput(DASH_LEFT);
		}
		else if (*(buttonPointer + 5) == 1){
			player1->controllerInput(DASH_RIGHT);
		}
		else if ((*(joystickPointer + 1) >= 0.90f) || (*(buttonPointer + 12) == 1)){
			player1->controllerInput(JUMP);
		}
		else if (*(buttonPointer + 2) == 1){
			player1->controllerInput(PUNCH);
		}
		else if (*buttonPointer == 1){
			player1->controllerInput(KICK);
		}
		else if (*(buttonPointer + 3) == 1){
			player1->controllerInput(LASER);
		}
		else if (*(buttonPointer + 1) == 1){
			player1->controllerInput(BLAST);
		}
	}
	else
	{
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
	}



	if (glfwJoystickPresent(1) == GL_TRUE){

		//initialize player 2 inputs
		int * joySize;
		joySize = new int;
		const float* joystickPointer = glfwGetJoystickAxes(1, joySize);
		int * buttonSize;
		buttonSize = new int;
		const unsigned char* buttonPointer = glfwGetJoystickButtons(1, buttonSize);

		//joystick player 2
		if ((*joystickPointer <= -0.25f) || (*(buttonPointer + 13) == 1)){
			player2->controllerInput(MOVE_LEFT);
		}
		else if ((*joystickPointer >= 0.25f) || (*(buttonPointer + 11) == 1)){
			player2->controllerInput(MOVE_RIGHT);
		}
		if ((*(joystickPointer + 1) <= -0.50f) || (*(buttonPointer + 10) == 1)){
			player2->controllerInput(BLOCK);
		}
		if (*(buttonPointer + 4) == 1){
			player2->controllerInput(DASH_LEFT);
		}
		else if (*(buttonPointer + 5) == 1){
			player2->controllerInput(DASH_RIGHT);
		}
		else if ((*(joystickPointer + 1) >= 0.90f) || (*(buttonPointer + 12) == 1)){
			player2->controllerInput(JUMP);
		}
		else if (*(buttonPointer + 2) == 1){
			player2->controllerInput(PUNCH);
		}
		else if (*buttonPointer == 1){
			player2->controllerInput(KICK);
		}
		else if (*(buttonPointer + 3) == 1){
			player2->controllerInput(LASER);
		}
		else if (*(buttonPointer + 1) == 1){
			player2->controllerInput(BLAST);
		}
	}
	else
	{
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
}


//Render the game
void Game::Render(void)
{
	firstPass->Activate();

	//Clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Update the view matrix
	mainCamera->Update(viewMatrix);

	//Activate the shader and render the player
	diffuseProgram->Activate();
	{
		firstPass->SetTexture(1);

		//Render all of the background objects
		for (unsigned int i = 0;i < BackgroundObjects.GetSize(); ++i)
		{
			firstPass->SetTexture(0);
			PreRender(BackgroundObjects.GetObjectAtIndex(i));
		}
		
		//Render the two player game objects
		PreRender(player1);
		PreRender(player2);

		//PreRender(light);
		
		//This is for debugging only and will be removed later on.
		PreRender(player1->GetCollisionBoxes());
		PreRender(player2->GetCollisionBoxes());
	}
	lightProgram->Deactivate();
	
	firstPass->Deactivate();
	

	glDisable(GL_DEPTH_TEST);
	
	//Enable new shader
	OutlineProgram->Activate();
	{
		//Set textures for the FBO
		firstPass->SetTexture(0);
		firstPass->BindColour(0);
		firstPass->SetTexture(1);
		firstPass->BindColour(1);
		firstPass->SetTexture(2);
		firstPass->BindDepth();
		
		glUniformMatrix4fv(uniform_outline_MVP, 1, 0, glm::value_ptr(glm::mat4(1)));
	
		fullScreenQuad->ActivateAndRender();
	}
	OutlineProgram->Deactivate();
	
	//Unbind the FBO textures
	firstPass->SetTexture(2);
	firstPass->UnbindTextures();
	firstPass->SetTexture(1);
	firstPass->UnbindTextures();
	firstPass->SetTexture(0);
	firstPass->UnbindTextures();
	
	//Render the HUD on top of everything else.
	RenderHUD();

	GLSLProgram::Deactivate();
	FrameBuffer::Deactivate();
	VertexBuffer::Deactivate();

	//Swap front and back buffers
	glfwSwapBuffers(gameWindow);
}

//Render the HUD of the game
void Game::RenderHUD(void)
{
	HUDProgram->Activate();
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, HUDBackgroundHandle);
		glUniform1i(uniform_HUD_texture, 0);

		glUniformMatrix4fv(uniform_HUD_MVP, 1, 0, glm::value_ptr(HUD->GetNode()->GetWorldTransform()));

		glUniform2fv(uniform_HUD_FaceDirection, 1, glm::value_ptr(glm::vec2(1.0f, 1.0f)));

		HUD->Render();

		for (unsigned int i = 0; i < 4; ++i)
		{
			glUniformMatrix4fv(uniform_HUD_MVP, 1, 0, glm::value_ptr(HUDBars[i]->GetNode()->GetWorldTransform()));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, HUDBarRedHandle);
			glUniform1i(uniform_HUD_texture, 0);

			if (i == 0 || i == 1)
				glUniform2fv(uniform_HUD_FaceDirection, 1, glm::value_ptr(glm::vec2(1.0f, 1.0f)));
			else
				glUniform2fv(uniform_HUD_FaceDirection, 1, glm::value_ptr(glm::vec2(-1.0f, 1.0f)));

			HUDBars[i]->Render();
		}
	}
	HUDProgram->Deactivate();
}

void Game::PreRender(GameObject* object)
{
	//TEMP
	for (unsigned int i = 0; i < 31; ++i)
	{
		glUniformMatrix4fv(uniform_boneMat[i], 1, 0, glm::value_ptr(glm::mat4(1)));
	}

	modelViewProjectionMatrix = object->UpdateModelViewProjection(projectionMatrix, viewMatrix);
	glUniformMatrix4fv(uniform_MVP, 1, 0, glm::value_ptr(modelViewProjectionMatrix));
	//glUniform3fv(uniform_LightPos, 1, glm::value_ptr(glm::inverse(object->GetNode()->GetWorldTransform()) * LIGHTPOS));
	//glUniform3fv(uniform_LightColour, 1, glm::value_ptr(glm::vec3(1, 1, 1)));
	
	//Pass in texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, object->GetTextureHandle());
	glUniform1i(uniform_texture, 0);
	
	//Pass in normal map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, object->GetNormalMapHandle());
	glUniform1i(uniform_normalMap, 1);

	////pass in qMap
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, qMap_handle);
	//glUniform1i(uniform_qMap, 2);

	object->Render();
}

//void Game::PreRender(GameObject* object, Light* light)
//{
//	modelViewProjectionMatrix = object->UpdateModelViewProjection(projectionMatrix, viewMatrix);
//	glUniformMatrix4fv(uniform_MVP, 1, 0, glm::value_ptr(modelViewProjectionMatrix));
//	glUniform3fv(uniform_LightPos, 1, glm::value_ptr(glm::inverse(object->GetNode()->GetWorldTransform()) * glm::vec4(light->GetNode()->GetWorldPosition(),1)));
//	glUniform3fv(uniform_LightColour, 1, glm::value_ptr(light->GetColour()));
//
//	PreRender(object);
//}

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