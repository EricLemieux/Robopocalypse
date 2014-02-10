#pragma once

//////////
//INCLUDES
//////////
#include "GameObject.h"

#include <GLFW\glfw3.h>

#include <iostream>

//////////
//GAME STATE
//
//Stores the different states the game can be in.
//////////
enum gameStates
{
	STATE_GAMEPLAY = 0,
	STATE_MAINMENU,
};


//////////
//GAME CLASS
//
//This class is the backbone of the game because most of the game is being handled here
//////////
class Game
{
public:
	//////////
	//CONSTRUCTORS
	//////////

	Game();
	~Game();

	//////////
	//INITIALISERS
	//////////

	//Open a glfw window with defined size
	void OpenWindow(int width, int height);

	//////////
	//SETTERS
	//////////



	//////////
	//GETTERS
	//////////

	//Get if the game is running
	inline bool GetIfRunning(void) { return isRunning; }

	//Get the game's current state
	inline gameStates GetState(void){ return GAME_STATE; }


	//////////
	//OTHERS
	//////////

	//Update the game
	void Update(void);

	//Render the game
	void Render(void);

private:

	//////////
	//DATA
	//////////

	//Stores if the game should be running
	bool isRunning;

	//Stores the current state of the game
	//Such as the gameplay state or the main menu state
	gameStates GAME_STATE;

	//The Window the game uses
	GLFWwindow *gameWindow;
};

