#include "Game.h"


bool spriteSortingFunction(Animation *s1, Animation *s2)
{
	// return true if s1's layerID is less than s2's layerID
	return (s1->layerID < s2->layerID);
}

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
		draw();
		update();
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
				//glMatrixMode(GL_PROJECTION);   //TODO
				//glLoadIdentity();
				//GLfloat ratio = static_cast<float>(window.getSize().x) / window.getSize().y;
				//glFrustum(-ratio, ratio, -1.f, 1.f, 1.f, 500.f);
				glViewport(0, 0, event.size.width, event.size.height);
                
					while(window.pollEvent(event)){

			//close window if event returns closed
			if(event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0,6)){
				window.close();
			} else if (event.type == sf::Event::Resized){
				glViewport(0,0,event.size.width, event.size.height);
			}

			if ((event.type == sf::Event::MouseButtonPressed)&&(event.mouseButton.button == mouse.Left)){  //TODO MOUSE
				//float x =  mouse.getPosition(window).x;
				//float y = mouse.getPosition(window).y;
				//

				//mouseClicks.push_back(sf::Vector2f(x,y));
			}

		}
		}
}


//void Game::gameEvents(sf::Event event){
//
//}


/* draw()
 * - this gets called automatically about 30 times per second
 * - this function just draws the sprites 
 */
void Game::draw(){
		/* sort the sprites by layerID so we draw them in the right order */
	std::sort(spriteListToDraw.begin(), spriteListToDraw.end(), spriteSortingFunction);
	///* pre-draw - setup the rendering */
	//PreDraw();

	/* draw - actually render to the screen */
	DrawGame();
	///* post-draw - after rendering, setup the next frame */
	//PostDraw();
}

/*
 * Pre-Draw() is for setting up things that need to happen in order to draw
 *    the game, i.e. sorting, splitting things into appropriate lists etc.
 */
//void Game::PreDraw()
//{
//	/* clear the screen */  //TEMP REMOVE?
//	//glViewport(0,0,stateInfo.windowWidth,stateInfo.windowHeight); 
//	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//	//glLoadIdentity(); // clear out the transformation matrix
//	//glEnable(GL_TEXTURE_2D); // turn on texturing
//
//	// if we update our timer it will tell us the elapsed time since the previous 
//	// frame that we rendered
//	//renderingTimer->tick(); TEMP
//}

/* 
 * DrawGame()
 *  - this is the actual drawing of the current frame of the game.
 */
void Game::DrawGame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisableClientState(GL_COLOR_ARRAY);

	window.clear();
	/* here is where your drawing stuff goes */

	//TODO

	window.pushGLStates();
	drawSprites();
	window.popGLStates();

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
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.f,0.f,camDist);
//	glColor3f(0.f,0.f,1.f);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//glGenTextures;
	glm::vec3 tempVertex;
	sf::Vector2f tempUV;
	glm::vec3 tempNorm;
	//for debugging
	//int a = assetList.objects[0].getVerSize();
	//int b = assetList.objects[0].getUVSize();
	//int c = assetList.objects[0].getNormSize();


	glPushMatrix();

	glTranslatef(xstep, 0.f, assetList.objects[2].getPosZ());

	glRotatef(yspeed, 0.0f, 1.f, 0.f);
	glRotatef(xspeed, 1.0f, 0.0f, 0.f);

	glBegin(GL_TRIANGLES);

	
	for(int j = 0; j < assetList.objects[2].getVerSize(); j++){
		tempVertex = assetList.objects[2].getVertex(j);
		tempUV = assetList.objects[2].getUV(j);
		tempNorm = assetList.objects[2].getNormal(j);
		
		glNormal3f( tempNorm.x,tempNorm.y,tempNorm.z);
		
		glTexCoord2f(tempUV.x,tempUV.y);
		
		glVertex3f( tempVertex.x,tempVertex.y,tempVertex.z);
	}
	
	glEnd();
	glPopMatrix();
}


void Game::drawSprites()
{
	
	/* better way */
	/* this is better because it doesn't matter how many sprites we have, they will always be drawn */
	std::vector<Animation*>::iterator it; 
	for(it=spriteListToDraw.begin(); it != spriteListToDraw.end();it++)
	{
		Animation *s = (*it);
		window.draw(s->sprite);
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

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D,hudTex);

	glBegin(GL_QUADS);
		glNormal3f( 0.f, 0.f, 1.f);
		glTexCoord2f(0.f,0.f);
		glVertex3f(-1.f, 0.9f,  -1.f);
		glTexCoord2f(1.f,0.f);
		glVertex3f( 1.f, 0.9f,  -1.f);
		glTexCoord2f(1.f,1.f);
		glVertex3f( 1.f,  -1.f,  -1.f);
		glTexCoord2f(0.f,1.f);
		glVertex3f(-1.f,  -1.f,  -1.f);
	glEnd();

	glEnable(GL_CULL_FACE);

	glPopMatrix();
}




/* update()
  - this function is essentially the game loop
    it gets called often and as such you
	don't actually need a "loop" to define the game
	it happens behind the scenes
  - think of this function as one iteration of the game loop
  - if you need to update physics calculations, sprite animation info,
    or sound etc, it goes in here
*/
void Game::update()
{ 
	
	//controller input
	checkLeftJoystick(0, player1);
    checkLeftJoystick(1, player2); 

	//Update the bounding boxes with the new positions.
	//TODO make seperate function
	assetList.boundingBoxes[0].setPos(player1.getPos());
	assetList.boundingBoxes[1].setPos(player2.getPos());

	player1.update(assetList.boundingBoxes, 0);
	player2.update(assetList.boundingBoxes, 1);

	//TODO Animations stuff/Interpolation
	frameTime = clock.getElapsedTime();
	clock.restart();

	float dt = frameTime.asSeconds();	
}

/* 
 * addSpriteToDrawList()
 * - this function simply pushes the sprite to the end of the list
 */
void Game::addSpriteToDrawList(Animation *s)
{
	if(s)
	{
		/* push the sprite to the back of the list */
		this->spriteListToDraw.push_back(s);
	}
}


/*************************************************/
/* INPUT - keyboard/mouse functions below        */
/*************************************************/
/* keyboardDown()
   - this gets called when you press a key down
   - you are given the key that was pressed
     and where the (x,y) location of the mouse is when pressed
*/
void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)  //TODO REMOVE
{
	switch(key) //TEMP move these keyboards into event handler
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}
}
/* keyboardUp()
   - this gets called when you lift a key up
   - you are given the key that was pressed
     and where the (x,y) location of the mouse is when pressed
*/
void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)  //TODOREMOVE
{
	switch(key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		exit(1);
		break;
	}
}

/*
 * mouseClicked
 * - this function is called when the mouse is clicked and it handles the 
 *   input state managment
 */
void Game::mouseClicked(int button, int state, int x, int y)  //TODO REMOVe
{
	//TEMP replace
}

/*
 * mouseMoved(x,y)
 * - this occurs only when the mouse is pressed down
 *   and the mouse has moved.  you are given the x,y locations
 *   in window coordinates (from the top left corner) and thus 
 *   must be converted to screen coordinates using the screen to window pixels ratio
 *   and the y must be flipped to make the bottom left corner the origin.
 */
void Game::mouseMoved(int x, int y)    //TODO REMOVE
{
	/* convert from window to screen pixel coordinates */
	input.currentX = x*stateInfo.ratioWidth;
	input.currentY = (stateInfo.windowHeight-y)*stateInfo.ratioHeight;
}