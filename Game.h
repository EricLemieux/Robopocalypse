//This is the main core of the game where most of the game actually happens

#pragma once

#include "Engine.h"
#include "HUD.h"



//Stores the game states
enum {
	STATE_GAMEPLAY=0, 
	STATE_MAINMENU
};

//Stores the information about the game, such as the window size and what the frames per second should be.
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

	//Frames per second
	float FPS;
}GameStateInfo;

//The actual game class, this is where eveything happens.
class Game
{
	public:
	Game(float fps);
	~Game(void);

	int gameState;

	//Initialize
	void initializeWindow();//glew is also init here
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

	//All of the drawing functions
	void draw();							//Main draw function, calls the other draw functions
	void drawHUD();							//Draws the HUD
	void DrawGame();						//Draws the players 
	void drawAssetList(Assets assetlist);	//Draws the world

	void healthManagement(float dt); // manages health

	//Update the game elements
	void update(); 


	/*********************************/
	/* DATA */
	/*********************************/

	
	// Create a clock for measuring the time elapsed TEMP
	//replace?
    //sf::Clock clock;
	//sf::Time frameTime;
	float frameTime;

	/* game state info */
	GameStateInfo stateInfo;
	//window width/height
	int width,height;

	float gameTime;


	//sf::ContextSettings contextSettings;
	//sf::RenderWindow window;
	GLFWwindow* window;

	//Should the game use the debug tools?
	bool debugTools;

	//Should the game draw the hitboxes?
    bool shouldDrawHitboxes;

	//Should the HUD be drawn?
	bool shouldDrawHUD;
	
	//Drawing obj
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	//In the main menu what item is selected
	int mainMenuSelection;

	//replace? this is made obsolete with key_callback
	//sf::Mouse mouse;

	glm::vec2 Position;

	Camera camera;

	//contains everything that isn't a player
	Assets assetList;
	Assets playerList;
	//player objects
	Player player1;
	Player player2;

	float timeBetweenFrames;
	
	/* HUD COMPONENTS */
	HUD gameHUD;

	GLuint mainMenuTex;

	NodeGraph graph;
};

//extern void drawHitboxes(std::vector<collisionObjects>  objects);
extern void drawHitboxes(Player &p1, Player &p2, Assets &assets);

extern void drawSquare(glm::vec3 pos, glm::vec3 size);