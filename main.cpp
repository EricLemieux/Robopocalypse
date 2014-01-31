/*
Robopocolypse

Created by:
	-Eric Lemieux		Lead Programmer
	-Brendan Ho			Producer / Programmer
	-Marc Evans			3D artist
	-Bishop Osborne		2D artist
	-Jonny Samaroo		Level Designer


		( Hi there)
		|    /
		\  /
		 V
    _(._.)_/'
	| | |
	  |_|
	 _| |_
*/


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////


#include "Game.h"

float const FPS = 30;
float const FRAME_DELAYSPRITE=1000/FPS;

Game *theGame;

//Starting the game
int main(){	

	//init the game
	theGame = new Game(FPS);
	theGame->setNewWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
	theGame->initializeWindow();
	
	//Based on the initial game state, decides what it needs to initalise right away
	if(theGame->gameState == STATE_MAINMENU)
	{
		theGame->initializeMainMenu();
	}
	else if(theGame->gameState == STATE_GAMEPLAY)
	{
		theGame->initializeGame();
	}
	
	//Start of the loop where it keeps the game open and calls the designated game loop for whatever the game state is.
	while(!glfwWindowShouldClose(theGame->window))
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
	glfwDestroyWindow(theGame->window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	//Exit the program
	return 0;
}
