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

	int gameState;

	//Initialize
	void initializeWindow();
	void initializeGame();
	void initializeMainMenu();

	void gameLoop();

	void gameEvent();

	void mainMenuLoop();


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

	/* draw/rendering routines */
	void draw(); // called from the main
	void drawHUD();

	void healthManagement(float dt); // manages health

	//void PreDraw(); // prior to drawing   //TEMP DEL, replace with sf::window display in one draw function
	void DrawGame(); // actual drawing the frame
	//void PostDraw(); // cleanup and prepare for next frame
	

	/* update routines, the game loop */
	void update(); // called from main frequently  //TEMP call physics, draw and functions


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

	float gameTime;
	bool cutsceneTest; //TEMP variable delete


	sf::ContextSettings contextSettings;
	sf::RenderWindow window;

	//Should the game use the debug tools?
	bool debugTools;


	//Should the game draw the hitboxes?
    bool shouldDrawHitboxes;

	//Should the HUD be drawn?
	bool shouldDrawHUD;
	
	//Drawing obj
	std::vector<glm::vec3> vertices;
	std::vector<sf::Vector2f> uvs;
	std::vector<glm::vec3> normals;

	//In the main menu what item is selected
	int mainMenuSelection;

	sf::Mouse mouse;

	sf::Vector2f Position;

	Camera camera;

	//contains everything that isn't a player
	Assets assetList;
	//player objects
	Player player1;
	Player player2;

	//1/30 for speed assuming FPS 30
	float t;

	//what were we using this for?
	GLfloat yspeed; 
	GLfloat zstep;
	GLfloat xspeed; 
	GLfloat xstep;
	GLfloat x2step; 
	GLfloat z2step; 
	GLfloat camDist;

	/* HUD COMPONENTS */

	float playerOneCurrentHealth;
	float playerOneLastHealth;
	float playerOneHealthDecayTimer;

	float playerTwoCurrentHealth;
	float playerTwoLastHealth;
	float playerTwoHealthDecayTimer;

	float playerOneCurrentSpecial;
	float playerOneLastSpecial;
	float playerOneSpecialDecayTimer;

	float playerTwoCurrentSpecial;
	float playerTwoLastSpecial;
	float playerTwoSpecialDecayTimer;

	bool healthCountdown;

	GLuint hudTex;
	GLuint mainMenuTex;

	NodeGraph graph;
};

//extern void drawHitboxes(std::vector<collisionObjects>  objects);
extern void drawHitboxes(Player &p1, Player &p2, Assets &assets);

extern void drawSquare(glm::vec3 pos, glm::vec3 size);