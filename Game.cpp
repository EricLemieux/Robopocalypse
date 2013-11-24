#include "Game.h"


/* constructor */
Game::Game(float fps)
{
	/* init state */
	stateInfo.gameState = STATE_GAMEPLAY;
	stateInfo.FPS=fps;

	yspeed = 0.0f;
	zstep = 0.0f;
	xspeed = 0.0f;
	xstep = 0.0f;
	x2step = 0.0f;
	z2step = 0.0f;
	camDist = -5.0f;

	// health stuff
	playerOneHealthDecayTimer = 0.0f;
	playerOneCurrentHealth = 100.0f;
	playerOneLastHealth = playerOneCurrentHealth;

	playerTwoHealthDecayTimer = 0.0f;
	playerTwoCurrentHealth = 100.0f;
	playerTwoLastHealth = playerTwoCurrentHealth;

	// special stuff
	playerOneSpecialDecayTimer = 0.0f;
	playerOneCurrentSpecial = 100.0f;
	playerOneLastSpecial = playerOneCurrentSpecial;

	playerTwoLastSpecial = 0.0f;
	playerTwoCurrentSpecial = 100.0f;
	playerTwoLastSpecial = playerTwoCurrentSpecial;

	healthCountdown = false;

	//Debuging tools
	//Should all be false in releases
	shouldDrawHitboxes = false;

	t = 0.03f;
}

/* destructor */
Game::~Game(void)
{
	/* deallocate memory and clean up here. if needed */
}

/* 
 * initializeGame()
 * - this function is called in the constructor to initialize everything related
 *   to the game, i..e loading sprites etc.
 * - MUST be called prior to any drawing/updating (you should add in checks to ensure this occurs in the right order)
 */
void Game::initializeGame()
{
	// Request a 32-bits depth buffer when creating the window
    contextSettings.depthBits = 32;

    // Create the main window
	window.create(sf::VideoMode(stateInfo.windowWidth, stateInfo.windowHeight), "No", sf::Style::Default, contextSettings);
    window.setVerticalSyncEnabled(true);

	window.setFramerateLimit(stateInfo.FPS);//TODO bring fps from main

	// Make it the active window for OpenGL calls
    window.setActive();


	assetList.LoadAssets("assets.txt");

	//add object to player
	player1 = Player(assetList.objects[0]);
	player2 = Player(assetList.objects[1]);
	assetList.objects.erase(assetList.objects.begin(),assetList.objects.begin()+2);
	assetList.boundingBoxes.erase(assetList.boundingBoxes.begin(),assetList.boundingBoxes.begin()+2);

	glClearDepth(1.f);
	glClearColor(0.5f, 0.5f,0.5f,1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LINEAR_MIPMAP_LINEAR);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, float(window.getSize().x)/float(window.getSize().y),2.f,2000.f);
	//gluLookAt(0,0,0,0,0,-10,0,1,0);
	glMatrixMode(GL_MODELVIEW);

	sf::Image hudMap;
	if(!hudMap.loadFromFile("resources/HUDback.jpg")){
		std::cout<<"error loading hud texture Game.cpp in void Game::initializeGame();\n";
	};

	glGenTextures(1,&hudTex);
	glBindTexture(GL_TEXTURE_2D,hudTex);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, hudMap.getSize().x, hudMap.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, hudMap.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	
	
}

void Game::gameLoop()
{
	while(window.isOpen()){
		
		gameEvent();
		update();
		draw();
		
	}
}

void Game::gameEvent(){
		sf::Event event; 
		// Process events
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();

            // Escape key : exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close();

            // Adjust the viewport when the window is resized
            if (event.type == sf::Event::Resized)

				glViewport(0, 0, event.size.width, event.size.height);
                
					while(window.pollEvent(event)){

			//close window if event returns closed
			if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0,6)){
				window.close();
			} else if (event.type == sf::Event::Resized){
				glViewport(0,0,event.size.width, event.size.height);
			}

			if ((event.type == sf::Event::MouseButtonPressed)&&(event.mouseButton.button == mouse.Left)){  //TODO MOUSE

			}

		}
		}
}


//draw
void Game::draw(){



	DrawGame();

}



//draw game
void Game::DrawGame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisableClientState(GL_COLOR_ARRAY);

	window.clear();

	drawHUD();

	glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
	glEnable(GL_TEXTURE_2D);
	
	

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f,WINDOW_WIDTH/WINDOW_HEIGHT,1.f,1000.f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	camera.setFocus(player1, player2);
	camera.update();

	player1.draw();
	player2.draw();
	drawFunc(assetList);

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	
	/* this makes it actually show up on the screen */
	//glutSwapBuffers();
	window.display();
}

//hardcoded get rid of it, currently draws the 3rd item in assets.txt which for the purposes of this method is the level
void Game::drawFunc(Assets assetList){
	for(int i = 0, size = assetList.objects.size(); i<size; ++i){
		assetList.objects[i].drawOBJ();
	}
}


//draw HUD
void Game::drawHUD(){
	glViewport(0,WINDOW_HEIGHT-HUD_HEIGHT,WINDOW_WIDTH,HUD_HEIGHT);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10,10,-10,10,1,1000);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glDisable(GL_CULL_FACE);

	glDisable(GL_TEXTURE_2D);

////////////////////////////////////////////////////////////////////////////////////////////////

	GLubyte halftone[] = {
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
    0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55};

/////////////////////////////////////////////////////////////////////////////////////////////////

	/* PLAYER ONE HEALTH */

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ratios for player health on the bar
	// 0 hp = -0.85 ratio on the hud
	// full hp = -0.127 ratio on the hud
	// fitting in the bars is 0.7 and 0.2

	// Sets the bar size to the health
	float temp1 = -0.127f;
	float temp2 = temp1 - 0.723f*(playerOneCurrentHealth/100);

	glEnable(GL_POLYGON_STIPPLE);
	
	// Make the health red
	

	glPolygonStipple(halftone);

	glColor3f(1.0, 0.0, 0.0);

	// Draw the health bar
	glBegin(GL_QUADS);
		glNormal3f( 0.f, 0.f, 1.f);
		glTexCoord2f(0.f,0.f);
		glVertex3f(temp1, 0.7f, -1.f);
		glTexCoord2f(1.f,0.f);
		glVertex3f(temp2,  0.7f, -1.f);
		glTexCoord2f(1.f,1.f);
		glVertex3f(temp2,  0.2f, -1.f);
		glTexCoord2f(0.f,1.f);
		glVertex3f(temp1, 0.2f, -1.f);
	glEnd();

	// Make the lost health yellow
	glColor3f(1.0, 1.0, 0.0);

	temp2 = temp1 - 0.723f*(playerOneLastHealth/100);

	// Draw the health bar
	glBegin(GL_QUADS);
		glNormal3f( 0.f, 0.f, 1.f);
		glTexCoord2f(0.f,0.f);
		glVertex3f(temp1, 0.7f, -1.f);
		glTexCoord2f(1.f,0.f);
		glVertex3f(temp2,  0.7f, -1.f);
		glTexCoord2f(1.f,1.f);
		glVertex3f(temp2,  0.2f, -1.f);
		glTexCoord2f(0.f,1.f);
		glVertex3f(temp1, 0.2f, -1.f);
	glEnd();

////////////////////////////////////////////////////////////////////////////////////////////////

	/* PLAYER ONE SPECIAL */

	// ratios for player health on the bar
	// 0 hp = -0.85 ratio on the hud
	// full hp = -0.127 ratio on the hud
	// fitting in the bars is 0.7 and 0.2

	// Sets the borders based on health
	temp1 = -0.127f;
	temp2 = temp1 - 0.723f*(playerOneCurrentSpecial/100);

	glColor3f(0.0, 0.0, 1.0);

	// Draws the special bar
	glBegin(GL_QUADS);
		glNormal3f( 0.f, 0.f, 1.f);
		glTexCoord2f(0.f,0.f);
		glVertex3f(temp1, -0.7f, -1.f);
		glTexCoord2f(1.f,0.f);
		glVertex3f(temp2, -0.7f, -1.f);
		glTexCoord2f(1.f,1.f);
		glVertex3f(temp2, -0.2f, -1.f);
		glTexCoord2f(0.f,1.f);
		glVertex3f(temp1, -0.2f, -1.f);
	glEnd();

	temp2 = temp1 - 0.723f*(playerOneLastSpecial/100);

	glColor3f(0.0, 1.0, 1.0);

	// Draws the lost health bar
	glBegin(GL_QUADS);
		glNormal3f( 0.f, 0.f, 1.f);
		glTexCoord2f(0.f,0.f);
		glVertex3f(temp1, -0.7f, -1.f);
		glTexCoord2f(1.f,0.f);
		glVertex3f(temp2, -0.7f, -1.f);
		glTexCoord2f(1.f,1.f);
		glVertex3f(temp2, -0.2f, -1.f);
		glTexCoord2f(0.f,1.f);
		glVertex3f(temp1, -0.2f, -1.f);
	glEnd();

////////////////////////////////////////////////////////////////////////////////////////////////

	/* PLAYER TWO HEALTH */

	// ratios for player health on the bar
	// 0 hp = -0.85 ratio on the hud
	// full hp = -0.127 ratio on the hud
	// fitting in the bars is 0.7 and 0.2

	// Sets the borders based on health
	temp1 = 0.127f;
	temp2 = temp1 + 0.723f*(playerTwoCurrentHealth/100);

	glColor3f(1.0, 0.0, 0.0);

	// Draws the health bar
	glBegin(GL_QUADS);
		glNormal3f( 0.f, 0.f, 1.f);
		glTexCoord2f(0.f,0.f);
		glVertex3f(temp1, 0.7f, -1.f);
		glTexCoord2f(1.f,0.f);
		glVertex3f(temp2,  0.7f, -1.f);
		glTexCoord2f(1.f,1.f);
		glVertex3f(temp2,  0.2f, -1.f);
		glTexCoord2f(0.f,1.f);
		glVertex3f(temp1, 0.2f, -1.f);
	glEnd();

	temp2 = temp1 + 0.723f*(playerTwoLastHealth/100);

	glColor3f(1.0, 0.0, 0.0);

	// Draws the lost health bar
	glBegin(GL_QUADS);
		glNormal3f( 0.f, 0.f, 1.f);
		glTexCoord2f(0.f,0.f);
		glVertex3f(temp1, 0.7f, -1.f);
		glTexCoord2f(1.f,0.f);
		glVertex3f(temp2,  0.7f, -1.f);
		glTexCoord2f(1.f,1.f);
		glVertex3f(temp2,  0.2f, -1.f);
		glTexCoord2f(0.f,1.f);
		glVertex3f(temp1, 0.2f, -1.f);
	glEnd();

	/////////////////////////////////////////////////

	/* PLAYER TWO SPECIAL */

	// ratios for player health on the bar
	// 0 hp = -0.85 ratio on the hud
	// full hp = -0.127 ratio on the hud
	// fitting in the bars is 0.7 and 0.2

	// Sets the borders based on health
	temp1 = 0.127f;
	temp2 = temp1 + 0.723f*(playerTwoCurrentSpecial/100);

	glColor3f(0.0, 0.0, 1.0);

	// Draws the health bar
	glBegin(GL_QUADS);
		glNormal3f( 0.f, 0.f, 1.f);
		glTexCoord2f(0.f,0.f);
		glVertex3f(temp1, -0.7f, -1.f);
		glTexCoord2f(1.f,0.f);
		glVertex3f(temp2, -0.7f, -1.f);
		glTexCoord2f(1.f,1.f);
		glVertex3f(temp2, -0.2f, -1.f);
		glTexCoord2f(0.f,1.f);
		glVertex3f(temp1, -0.2f, -1.f);
	glEnd();

	temp2 = temp1 + 0.723f*(playerTwoLastSpecial/100);

	glColor3f(0.0, 1.0, 1.0);

	// Draws the lost special bar
	glBegin(GL_QUADS);
		glNormal3f( 0.f, 0.f, 1.f);
		glTexCoord2f(0.f,0.f);
		glVertex3f(temp1, -0.7f, -1.f);
		glTexCoord2f(1.f,0.f);
		glVertex3f(temp2, -0.7f, -1.f);
		glTexCoord2f(1.f,1.f);
		glVertex3f(temp2, -0.2f, -1.f);
		glTexCoord2f(0.f,1.f);
		glVertex3f(temp1, -0.2f, -1.f);
	glEnd();

////////////////////////////////////////////////////////////////////////////////////////////////
	glDisable(GL_POLYGON_STIPPLE);
	/////////////////////////////////////////////////
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D,hudTex);

	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_QUADS);
		glNormal3f( 0.f, 0.f, 1.f);
		glTexCoord2f(0.f,0.f);
		glVertex3f(-1.f, 1.f,  -1.f);
		glTexCoord2f(1.f,0.f);
		glVertex3f( 1.f, 1.f,  -1.f);
		glTexCoord2f(1.f,1.f);
		glVertex3f( 1.f,  -1.f,  -1.f);
		glTexCoord2f(0.f,1.f);
		glVertex3f(-1.f,  -1.f,  -1.f);
	glEnd();

//////////////////////////////////////////////////////////

	glEnable(GL_CULL_FACE);
	glPopMatrix();
}

void Game::healthManagement(float dt)
{
	
	if (playerOneLastHealth != playerOneCurrentHealth)
	{
		playerOneHealthDecayTimer += dt*0.2f;

		playerOneLastHealth = ((1-playerOneHealthDecayTimer)*playerOneLastHealth) + ((playerOneHealthDecayTimer)*playerOneCurrentHealth);

	}

	if (playerOneHealthDecayTimer >= 1)
	{
		playerOneHealthDecayTimer = 0.0f;

		playerOneLastHealth = playerOneLastHealth;
	}

	if (playerTwoLastHealth != playerTwoCurrentHealth)
	{
		playerTwoHealthDecayTimer += dt*0.2f;

		playerTwoLastHealth = ((1-playerTwoHealthDecayTimer)*playerTwoLastHealth) + ((playerTwoHealthDecayTimer)*playerTwoCurrentHealth);

	}

	if (playerTwoHealthDecayTimer >= 1)
	{
		playerTwoHealthDecayTimer = 0.0f;

		playerTwoLastHealth = playerOneLastHealth;
	}

	if (playerOneLastSpecial != playerOneCurrentSpecial)
	{
		playerOneSpecialDecayTimer += dt*0.2f;

		playerOneLastSpecial = ((1-playerOneSpecialDecayTimer)*playerOneLastSpecial) + ((playerOneSpecialDecayTimer)*playerOneCurrentSpecial);

	}

	if (playerOneSpecialDecayTimer >= 1)
	{
		playerOneSpecialDecayTimer = 0.0f;

		playerOneLastSpecial = playerOneCurrentSpecial;
	}

	if (playerTwoLastSpecial != playerTwoCurrentSpecial)
	{
		playerTwoSpecialDecayTimer += dt*0.2f;

		playerTwoLastSpecial = ((1-playerTwoSpecialDecayTimer)*playerTwoLastSpecial) + ((playerTwoSpecialDecayTimer)*playerTwoCurrentSpecial);

	}

	if (playerTwoSpecialDecayTimer >= 1)
	{
		playerTwoHealthDecayTimer = 0.0f;

		playerTwoLastSpecial = playerTwoCurrentSpecial;
	}
}




//UPDATE gamestate
void Game::update()
{ 
	//controller input
	checkLeftJoystick(0, player1);
    //checkLeftJoystick(1, player2); 


	player1.update(assetList.boundingBoxes, 0,t);
	player2.update(assetList.boundingBoxes, 1,t);

	//TODO Animations stuff/Interpolation
	frameTime = clock.getElapsedTime();
	clock.restart();

	float dt = frameTime.asSeconds();

	drawHUD();

	healthManagement(dt);
	
}



//Draw the hitboxes for everything
void drawHitboxes(std::vector<collisionObjects>  objects)
{
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glEnable(GL_COLOR_MATERIAL);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glBegin(GL_QUADS);
		for(unsigned int i = 0; i < objects.size(); ++i)
		{
			//glTranslatef(assetLis
			//glBegin(GL_QUADS);
			//Back face
			glVertex3f(objects[i].getPos().x - objects[i].getSize().x/2,	objects[i].getPos().y + objects[i].getSize().y/2,	objects[i].getPos().z - objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x + objects[i].getSize().x/2,	objects[i].getPos().y + objects[i].getSize().y/2,	objects[i].getPos().z - objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x + objects[i].getSize().x/2,	objects[i].getPos().y - objects[i].getSize().y/2,	objects[i].getPos().z - objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x - objects[i].getSize().x/2,	objects[i].getPos().y - objects[i].getSize().y/2,	objects[i].getPos().z - objects[i].getSize().z/2);
			//Front face
			glVertex3f(objects[i].getPos().x - objects[i].getSize().x/2,	objects[i].getPos().y + objects[i].getSize().y/2,	objects[i].getPos().z + objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x + objects[i].getSize().x/2,	objects[i].getPos().y + objects[i].getSize().y/2,	objects[i].getPos().z + objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x + objects[i].getSize().x/2,	objects[i].getPos().y - objects[i].getSize().y/2,	objects[i].getPos().z + objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x - objects[i].getSize().x/2,	objects[i].getPos().y - objects[i].getSize().y/2,	objects[i].getPos().z + objects[i].getSize().z/2);

			//Top face
			glVertex3f(objects[i].getPos().x - objects[i].getSize().x/2,	objects[i].getPos().y + objects[i].getSize().y/2,	objects[i].getPos().z + objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x + objects[i].getSize().x/2,	objects[i].getPos().y + objects[i].getSize().y/2,	objects[i].getPos().z + objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x + objects[i].getSize().x/2,	objects[i].getPos().y + objects[i].getSize().y/2,	objects[i].getPos().z - objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x - objects[i].getSize().x/2,	objects[i].getPos().y + objects[i].getSize().y/2,	objects[i].getPos().z - objects[i].getSize().z/2);
			//Bottom face
			glVertex3f(objects[i].getPos().x - objects[i].getSize().x/2,	objects[i].getPos().y - objects[i].getSize().y/2,	objects[i].getPos().z + objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x + objects[i].getSize().x/2,	objects[i].getPos().y - objects[i].getSize().y/2,	objects[i].getPos().z + objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x + objects[i].getSize().x/2,	objects[i].getPos().y - objects[i].getSize().y/2,	objects[i].getPos().z - objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x - objects[i].getSize().x/2,	objects[i].getPos().y - objects[i].getSize().y/2,	objects[i].getPos().z - objects[i].getSize().z/2);
			
			//Right face
			glVertex3f(objects[i].getPos().x + objects[i].getSize().x/2,	objects[i].getPos().y + objects[i].getSize().y/2,	objects[i].getPos().z + objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x + objects[i].getSize().x/2,	objects[i].getPos().y + objects[i].getSize().y/2,	objects[i].getPos().z - objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x + objects[i].getSize().x/2,	objects[i].getPos().y - objects[i].getSize().y/2,	objects[i].getPos().z + objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x + objects[i].getSize().x/2,	objects[i].getPos().y - objects[i].getSize().y/2,	objects[i].getPos().z - objects[i].getSize().z/2);
			//Left face
			glVertex3f(objects[i].getPos().x - objects[i].getSize().x/2,	objects[i].getPos().y + objects[i].getSize().y/2,	objects[i].getPos().z + objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x - objects[i].getSize().x/2,	objects[i].getPos().y + objects[i].getSize().y/2,	objects[i].getPos().z - objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x - objects[i].getSize().x/2,	objects[i].getPos().y - objects[i].getSize().y/2,	objects[i].getPos().z + objects[i].getSize().z/2);
			glVertex3f(objects[i].getPos().x - objects[i].getSize().x/2,	objects[i].getPos().y - objects[i].getSize().y/2,	objects[i].getPos().z - objects[i].getSize().z/2);

			//glEnd();
		}
	glEnd();
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glPopMatrix();
}