////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Game.h"
/******************************************/
/* GLOBAL VARIABLES */
/******************************************/

/*********************************/
/* GLOBAL SPRITE INFO */
/*********************************/
float const FPS = 30;
float const FRAME_DELAYSPRITE=1000/FPS;

Game *theGame;

int main(){	
	theGame = new Game(FPS);
	theGame->setNewWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
	theGame->initializeWindow();
	

	if(theGame->gameState == STATE_MAINMENU)
	{
		theGame->initializeMainMenu();
	}
	else if(theGame->gameState == STATE_GAMEPLAY)
	{
		theGame->initializeGame();
	}
	
	while(theGame->window.isOpen())
	{
		if(theGame->gameState == STATE_MAINMENU)
		{
			theGame->mainMenuLoop();
		}
		if(theGame->gameState == STATE_GAMEPLAY)
		{
			theGame->gameLoop();
		}
	}

	return 0;
}
