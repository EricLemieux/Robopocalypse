#pragma once

#include "Engine.h"

/* these are enumerated types (just names) of game states
   that the game can be in at any moment.  These help you
   manage the flow of the game.  Especially if you want 
   to do different updates (call different update functions)
   or have different behaviour for input (i.e. mouse and keyboard
   functions might have different behaviour in-game (STATE_GAMEPLAY)
   versus when in the Menu (STATE_MAINMENU)
*/
enum {
	STATE_GAMEPLAY=0, 
	STATE_MAINMENU, 
	STATE_CREDITS
};

// input information related to mouse
// you could add keyboard info in here too if you need to know if a key is currently pressed down
typedef struct _InputInfo
{
	bool mouseDown; // is the mouse down?
	int button; // which mouse button
	float clickX,clickY; // where the mouse was clicked
	float currentX,currentY;
}InputInfo;

typedef struct _GameStateInfo
{
	int gameState; // the game state, see enum's above

	// window width/height are the actual size of the window
	int windowWidth, windowHeight;

	// screen width/height are the apparent size of the window
	// i.e. you can have a 640x480 game screen stretched to 1280x1024
	// your calculations for actual pixel locations will depend on the ratio
	//   between screen and window width/height
	int screenWidth, screenHeight;  

	float ratioWidth,ratioHeight;

	float FPS;
}GameStateInfo;

class Game
{
	public:
	Game(float fps);
	~Game(void);

	//Initialize
	void initializeGame();

	void gameLoop();

	void gameEvent();

	//void gameEvents(sf::Event event); TEMP change to event function
	// add a sprite to the draw list
	// only sprites in the draw list will be drawn

		/* convenience functions */
	// screen size is the apparent size in pixels
	// i.e. this is what your game calculations should use
	void setScreenSize(int w, int h)
	{
		stateInfo.screenWidth = w;
		stateInfo.screenHeight = h;
	}
	// sets the screen-to-window pixel coordinates ratio
	void updateScreenRatio()
	{
		stateInfo.ratioWidth = stateInfo.screenWidth/(float)stateInfo.windowWidth;
		stateInfo.ratioHeight = stateInfo.screenHeight/(float)stateInfo.windowHeight;
		/* these are only really used for the mouse function
		   to get actual screen pixel coordinates from the window coordinates */
	}

	// sets the window size in pixels for the window itself
	void setNewWindowSize(int width,int height)
	{
		stateInfo.windowWidth = width;
		stateInfo.windowHeight = height;
	}

	void addSpriteToDrawList(Animation *s);

	/* draw/rendering routines */
	void draw(); // called from the main

	//void PreDraw(); // prior to drawing   //TEMP DEL, replace with sf::window display in one draw function
	void DrawGame(); // actual drawing the frame
	//void PostDraw(); // cleanup and prepare for next frame
	
	void drawSprites(); // draw the sprite list

	/* update routines, the game loop */
	void update(); // called from main frequently  //TEMP call physics, draw and functions
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

	void drawFunc(Assets assetlist);


	/*********************************/
	/* DATA */
	/*********************************/

	
	// Create a clock for measuring the time elapsed TEMP
    sf::Clock clock;
	sf::Time frameTime;

	/* game state info */
	GameStateInfo stateInfo;

	InputInfo input;

	/* sprite list to draw */
	std::vector<Animation*> spriteListToDraw;

	sf::ContextSettings contextSettings;
	sf::RenderWindow window;
	
	/* you could have more lists of sprite pointers */
	/* such as spritesToUpdateForPhysics  
	   or      spritesToUpdateForCollisions 
	   etc....
    */
	Animation *FATCAT;
	Animation *BURD;
	/* timer's for rendering and animation/physics update */
	//Timer *renderingTimer;
	//Timer *updateTimer; // for physics/collisions etc.  //TEMP sf clock?

	sf::Texture Spritetexture; //TEMP, I c
	sf::Sprite temp;
	
	//Drawing obj
	std::vector<sf::Vector3f> vertices;
	std::vector<sf::Vector2f> uvs;
	std::vector<sf::Vector3f> normals;


	sf::Mouse mouse;

	sf::Vector2f Position;


	Camera camera;

	Assets assetList;
	Player player1;
	Player player2;

	GLfloat yspeed; 
	GLfloat zstep;
	GLfloat xspeed; 
	GLfloat xstep;
	GLfloat x2step; 
	GLfloat z2step; 
	GLfloat camDist;
};

