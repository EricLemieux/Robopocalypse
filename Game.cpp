#include "Game.h"

//glfw callbacks
static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

/* constructor */
Game::Game(float fps)
{
	gameState = STATE_GAMEPLAY;

	//init onscreen graph
	graph.init();

	//Debugging tools
	//Should all be false in releases
	debugTools = false;
	if(debugTools)
	{
		shouldDrawHitboxes = true;
	}
	else
	{
		shouldDrawHitboxes = false;
	}

	//TODO make more exact, maybe get the difference in time each update from sfml.
	timeBetweenFrames = 1.0f/30.0f;
}

// destructor
Game::~Game(void)
{
}

void Game::initializeWindow()
{
	//replaced
	// Request a 32-bits depth buffer when creating the window
    //contextSettings.depthBits = 32;
    // Create the main window
	//window.create(sf::VideoMode(stateInfo.windowWidth, stateInfo.windowHeight), "RoboPocalypse", sf::Style::Default, contextSettings);
    //window.setVerticalSyncEnabled(true);
	//
	//window.setFramerateLimit(stateInfo.FPS);//TODO bring fps from main
	//
	//// Make it the active window for OpenGL calls
    //window.setActive();

	glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
	window = glfwCreateWindow(stateInfo.windowWidth, stateInfo.windowHeight, "Robobopocalypse", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

	//init glew
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		fprintf(stderr,"Failed to init GLEW\n");
	}

	//Init DevIL
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

/* 
 * initializeGame()
 * - this function is called in the constructor to initialize everything related
 *   to the game, i..e loading sprites etc.
 * - MUST be called prior to any drawing/updating (you should add in checks to ensure this occurs in the right order)
 */
void Game::initializeGame()
{
	assetList.objects.clear();
	assetList.LoadAssets("assets.txt");

	playerList.objects.clear();
	playerList.LoadAssets("assetsPlayers.txt");

	//add object to player
	player1 = Player(playerList.objects[0]);
	player2 = Player(playerList.objects[1]);

	glClearDepth(1.f);
	glClearColor(0.5f, 0.5f,0.5f,1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LINEAR_MIPMAP_LINEAR);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	//change this to glfwGetFramebufferSize later
	glfwGetWindowSize(window,&width, &height);
	
	//Set up the camera and projection matrix
	camFOV					= 90.0f;
	camNearClippingPlane	= 0.1f;
	camFarClippingPlane		= 1000.0f;
	projectionMat = glm::perspective(camFOV, (float)stateInfo.screenHeight / (float)stateInfo.screenHeight, camNearClippingPlane, camFarClippingPlane);

	//Disable the HUD until the cutscene is done
	shouldDrawHUD = false;

	//for debugging
	gameTime = 0.0f;
}

void Game::initializeMainMenu()
{
	mainMenuSelection = 1;

	glClearDepth(1.f);
	glClearColor(0.0f, 0.5f,0.5f,1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LINEAR_MIPMAP_LINEAR);
	glDepthFunc(GL_LESS);
	
	glfwGetWindowSize(window,&width, &height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,width,0,height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//replace?
	//main menu background
	//sf::Image mainmenuImageMap;
	//if(!mainmenuImageMap.loadFromFile("resources/Menu_with_Buttons.png")){
	//	std::cout<<"error loading main menu texture Game.cpp in void Game::initializeGame();\n";
	//}

	//glGenTextures(1,&mainMenuTex);
	//glBindTexture(GL_TEXTURE_2D,mainMenuTex);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, mainmenuImageMap.getSize().x, mainmenuImageMap.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, mainmenuImageMap.getPixelsPtr());
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void Game::gameLoop()
{
	gameEvent();
	update();
	draw();
}

void Game::mainMenuLoop()
{
	////Event loop
	gameEvent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisableClientState(GL_COLOR_ARRAY);

	glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
	glEnable(GL_TEXTURE_2D);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glfwGetWindowSize(window,&width, &height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, width/height, 1, 1000);
	gluLookAt(0,0,0,
			0,0,-50,
			0,1,0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//replace?
	//Draw the background
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, mainMenuTex);
	//drawSquare(glm::vec3(0,0,0.5), glm::vec3(100,100,100));
	//glDisable(GL_TEXTURE_2D);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Draw the selection
	glm::vec3 pos;
	glColor4f(1,1,0,0.4);
	if(mainMenuSelection == 0)
	{
		pos = glm::vec3(6.8, -4.5, -9);
	}
	else if(mainMenuSelection == 1)
	{
		pos = glm::vec3(-6.8, -4.5, -9);
	}
	drawSquare(pos, glm::vec3(1.1,2.2,0));
	glColor4f(1,1,1,1);

	//replaced
	glfwSwapBuffers(window);
	//window.display();
}

void Game::gameEvent(){
	//replaced

	

	glfwPollEvents();
	if(gameState == STATE_GAMEPLAY){
		gameTime = 5.f;
	}
		//sf::Event event; 

	//the while loop below should happen in key_callback at top of game.cpp
		// Process events
       /* while (window.pollEvent(event))
        {
			//below is obsolete, covered in key_callback
			////close window if event returns closed
			//if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0,6)){
			//	window.close();
			//} else if (event.type == sf::Event::Resized){
			//	glViewport(0,0,event.size.width, event.size.height);
			//}

			//if ((event.type == sf::Event::MouseButtonPressed)&&(event.mouseButton.button == mouse.Left)){  //TODO MOUSE
			//
			//}

			if(gameState == STATE_MAINMENU)
			{
				glm::vec3 mousePos = glm::vec3(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, 10);

				//Move left and right in the main menu
				if(sf::Joystick::getAxisPosition(0,sf::Joystick::X) < -40		|| isBoxBoxColliding(glm::vec3(172.5,590,10),glm::vec3(87.5*2,100*2,0), mousePos, glm::vec3(2,2,10)))
				{
					if(mainMenuSelection < 1)
					{
						mainMenuSelection ++;
					}
				}
				else if(sf::Joystick::getAxisPosition(0,sf::Joystick::X) > 40	|| isBoxBoxColliding(glm::vec3(1220,590,10),glm::vec3(87.5*2,100*2,0), mousePos, glm::vec3(2,2,10)))
				{
					if(mainMenuSelection > 0)
					{
						mainMenuSelection --;
					}
				}

				//Make a selection in the main menu
				if(sf::Joystick::isButtonPressed(0,0))
				{
					if(mainMenuSelection == 0)
					{
						exit(100);
					}
					else if(mainMenuSelection == 1)
					{
						gameState = STATE_GAMEPLAY;
						initializeGame();
					}
				}

				if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if(isBoxBoxColliding(glm::vec3(172.5,590,10),glm::vec3(87.5*2,100*2,0), mousePos, glm::vec3(2,2,10)))
					{
						gameState = STATE_GAMEPLAY;
						initializeGame();
					}
					if(isBoxBoxColliding(glm::vec3(1220,590,10),glm::vec3(87.5*2,100*2,0), mousePos, glm::vec3(2,2,10)))
					{
						exit(100);
					}
				}
			}
			else if(gameState == STATE_GAMEPLAY)
			{
				if(sf::Joystick::isButtonPressed(0,7))	
				{
					gameTime = 5.0f;
				}
			}
		}*/
	
}


//draw
void Game::draw(){
	//Clear the depth and colour buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisableClientState(GL_COLOR_ARRAY);

	//Draw the HUD in it's own viewport
	if(shouldDrawHUD)
	{
		//drawHUD();
		gameHUD.draw();
	}

	DrawGame();
}



//draw game
void Game::DrawGame()
{
	glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);	
	
	//camera update is in here in order to track player position
	viewMat = camera.update();

	drawAssetList(assetList);

	player1.draw();
	player2.draw();

	if(shouldDrawHitboxes)
	{
		//Draw the graph used by A*
		graph.draw();

		//Draw the hit boxes
		drawHitboxes(player1, player2, assetList);
	}

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	
	//Swap front and back buffers
	glfwSwapBuffers(window);
}

//TODO: KILL
//Draws the asset list full of objects
void Game::drawAssetList(Assets assetList){
	for(int i = 0, size = assetList.objects.size(); i<size; ++i){
		assetList.objects[i].drawOBJ();
	}
}

//UPDATE
void Game::update()
{
	//controller input
	checkController(0, player1);
    checkController(1, player2); 

	//Update the players
	player1.update(assetList, 0,timeBetweenFrames, player2, graph);
	player2.update(assetList, 1,timeBetweenFrames, player1, graph);

	frameTime = glfwGetTime();
	glfwSetTime(0);
	//replaced
	//frameTime = clock.getElapsedTime();
	//clock.restart();

	//float dt = frameTime.asSeconds();

	//Animate the camera for the cutscenes
	if(gameTime <= 5.0f)
	{
		gameTime += frameTime/5;
	}


	//Animate the intro cutscene
	if(player1.getHealth() > 0 && player2.getHealth() > 0)
	{
		if(gameTime <= 1.0f)
		{
			player1.setStunCooldown(2.0 * 30);
			player2.setStunCooldown(2.0 * 30);
			camera.setPos(glm::vec3(50, 10, 10), glm::vec3(-50, 10, 10), gameTime, 1.0f);
			camera.setTarget(glm::quat(0,player1.getPos()), glm::quat(0,player2.getPos()), gameTime, 1.0f);
		}
		else if(gameTime <= 1.5f)
		{
			player1.setStunCooldown(2.0 * 30);
			player2.setStunCooldown(2.0 * 30);
			camera.setFocus(player1);
		}
		else if(gameTime <= 2.0f)
		{
			player1.setStunCooldown(2.0 * 30);
			player2.setStunCooldown(2.0 * 30);
			camera.setFocus(player2);
		}
		else
		{
			camera.setFocus(player1, player2);
			shouldDrawHUD = true;
		}
	}

	//Check to see if a player has won the game
	if(player1.getHealth() <= 0)
	{
		//player 2 wins
		shouldDrawHUD = false;
		
		if(gameTime > 2.0f)
		{
			gameTime = 0.0f;
		}
		else if(gameTime > 1.7f)
		{
			gameState = STATE_MAINMENU;
			initializeMainMenu();
		}
		player1.setStunCooldown(2.0 * 30);
		player2.setStunCooldown(2.0 * 30);

		if(player1.getHealth() > -40)
		{
			player1.setHealth(player1.getHealth() - 1);
			player1.Death();
		}
		
		//Set the camera infront of player 2
		camera.setPos(camera.getCamPos(), glm::vec3(player2.getPos().x, player2.getPos().y, player2.getPos().z + 20), gameTime, 1.0f);
		camera.setTarget(player2.getPos());
	}
	else if(player2.getHealth() <= 0)
	{
		//player 1 wins
		shouldDrawHUD = false;
		if(gameTime > 2.0f)
		{
			gameTime = 0.0f;
		}
		else if(gameTime > 1.7f)
		{
			gameState = STATE_MAINMENU;
			initializeMainMenu();
		}
		player1.setStunCooldown(2.0 * 30);
		player2.setStunCooldown(2.0 * 30);
		
		if(player2.getHealth() > -40)
		{
			player2.setHealth(player2.getHealth() - 1);
			player2.Death();
		}

		//Set the camera infront of player 1
		camera.setPos(camera.getCamPos(), glm::vec3(player1.getPos().x, player1.getPos().y, player1.getPos().z + 20), gameTime, 1.0f);
		camera.setTarget(player1.getPos());
	}

	gameHUD.update(&player1, &player2, timeBetweenFrames/2.0f);

	//drawHUD();

	//TODO kill
	//healthManagement(dt);
	
}

void drawCube()
{
	glBegin(GL_QUADS);
		//Back face
		glVertex3f( -1, +1, -1);
		glVertex3f( +1, +1, -1);
		glVertex3f( +1, -1, -1);
		glVertex3f( -1, -1, -1);
		//Front face
		glVertex3f( -1, +1, +1);
		glVertex3f( +1, +1, +1);
		glVertex3f( +1, -1, +1);
		glVertex3f( -1, -1, +1);

		//Top face
		glVertex3f( -1, +1, -1);
		glVertex3f( +1, +1, -1);
		glVertex3f( +1, +1, +1);
		glVertex3f( -1, +1, +1);
		//Bottom face
		glVertex3f( -1, -1, -1);
		glVertex3f( +1, -1, -1);
		glVertex3f( +1, -1, +1);
		glVertex3f( -1, -1, +1);

		//Right face
		glVertex3f( +1, +1, +1);
		glVertex3f( +1, +1, -1);
		glVertex3f( +1, -1, -1);
		glVertex3f( +1, -1, +1);
		//Left face
		glVertex3f( -1, +1, +1);
		glVertex3f( -1, +1, -1);
		glVertex3f( -1, -1, -1);
		glVertex3f( -1, -1, +1);
	glEnd();
}

void drawPlayerBoundingBoxes(Player &tempPlayer)
{
	//Reset colour
	glColor4f(1, 1, 1, 1);
	glDisable(GL_TEXTURE_2D);

	//Draw the main player bounding box
	glPushMatrix();
	glTranslatef(tempPlayer.getPos().x, tempPlayer.getPos().y, tempPlayer.getPos().z);
	glScalef(tempPlayer.getObject().getHitBox().getSize().x/2, tempPlayer.getObject().getHitBox().getSize().y/2, tempPlayer.getObject().getHitBox().getSize().z/2);
	drawCube();
	glPopMatrix();

	//Draw the attack's bounding box
	glColor3f(1,0,0);

	//Drawing punch
	glPushMatrix();
	glTranslatef(tempPlayer.attackFist.getPos().x, tempPlayer.attackFist.getPos().y, tempPlayer.attackFist.getPos().z);
	glScalef(tempPlayer.attackFist.getSize().x/2, tempPlayer.attackFist.getSize().y/2, tempPlayer.attackFist.getSize().z/2);
	drawCube();
	glPopMatrix();

	//Drawing kick
	glPushMatrix();
	glTranslatef(tempPlayer.attackKick.getPos().x, tempPlayer.attackKick.getPos().y, tempPlayer.attackKick.getPos().z);
	glScalef(tempPlayer.attackKick.getSize().x/2, tempPlayer.attackKick.getSize().y/2, tempPlayer.attackKick.getSize().z/2);
	drawCube();
	glPopMatrix();

	//Drawing ranged attack
	glPushMatrix();
	glTranslatef(tempPlayer.attackRange.getPos().x, tempPlayer.attackRange.getPos().y, tempPlayer.attackRange.getPos().z);
	glScalef(tempPlayer.attackRange.getSize().x/2, tempPlayer.attackRange.getSize().y/2, tempPlayer.attackRange.getSize().z/2);
	drawCube();
	glPopMatrix();

	//draw block field
	glPushMatrix();
	glTranslatef(tempPlayer.blockBox.getPos().x, tempPlayer.blockBox.getPos().y, tempPlayer.blockBox.getPos().z);
	glScalef(tempPlayer.blockBox.getSize().x/2, tempPlayer.blockBox.getSize().y/2, tempPlayer.blockBox.getSize().z/2);
	drawCube();
	glPopMatrix();

	//Reset colour
	glColor4f(1, 1, 1, 1);
	glDisable(GL_TEXTURE_2D);
}

//Draw the hitboxes for everything
//void drawHitboxes(std::vector<collisionObjects>  objects)
extern void drawHitboxes(Player &p1, Player &p2, Assets &assets)
{
	//glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glEnable(GL_COLOR_MATERIAL);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	//glBegin(GL_QUADS);

	//Draw player1's bouding box
	drawPlayerBoundingBoxes(p1);


	//Draw player2's bouding box
	drawPlayerBoundingBoxes(p2);

	//Draw asset list
	for(unsigned int i = 0; i < assets.objects.size(); ++i)
		{
			glPushMatrix();
			//glTranslatef(assets.objects[i].getPosX(), assets.objects[i].getPosY(), assets.objects[i].getPosZ());
			glTranslatef(assets.objects[i].getHitBox().getPos().x, assets.objects[i].getHitBox().getPos().y, assets.objects[i].getHitBox().getPos().z);
			glScalef(assets.objects[i].getHitBox().getSize().x/2, assets.objects[i].getHitBox().getSize().y/2, assets.objects[i].getHitBox().getSize().z/2);
			drawCube();
			glPopMatrix();
		}
	//glEnd();
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	//glPopMatrix();

	//Reset colour
	glColor4f(1, 1, 1, 1);
	glDisable(GL_TEXTURE_2D);
}

extern void drawSquare(glm::vec3 pos, glm::vec3 size)
{
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glScalef(size.x, size.y, size.z);
	glDisable(GL_CULL_FACE);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-1, +1, -1);
		
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(+1, +1, -1);
		
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(+1, -1, -1);
		
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-1, -1, -1);
	glEnd();
	glPopMatrix();
}