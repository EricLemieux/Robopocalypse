////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Game.h"
/******************************************/
/* GLOBAL VARIABLES */
/******************************************/
#define WINDOW_SCREEN_WIDTH 1400
#define WINDOW_SCREEN_HEIGHT 800
#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 800
/*********************************/
/* GLOBAL SPRITE INFO */
/*********************************/
float const FPS = 30;
float const FRAME_DELAYSPRITE=1000/FPS;

Game *theGame;

int main(){	
	theGame = new Game(FPS);
	theGame->setNewWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
	theGame->initializeGame();
	
	theGame->gameLoop();

	return 0;
}
