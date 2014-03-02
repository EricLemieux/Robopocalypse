#pragma once

//////////
//INCLUDES
//////////
#include <GL\glew.h>

#include "SceneGraph.h"
#include "GameObject.h"
#include "GLSL.h"
#include "Assets.h"

#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <glm\gtx\euler_angles.hpp>

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

	//Initialises for gameplay
	void initGameplay(void);

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

	//Update the variables for the shaders before rendering the Game Object
	void PreRender(GameObject* object);

private:

	//////////
	//DATA
	//////////

	Node *world;
	Node *sceneGraph;
	
	//Game Objects
	GameObject *player1, *player2;

	Assets BackgroundObjects;

	//Stores if the game should be running
	bool isRunning;

	//Stores the current state of the game
	//Such as the gameplay state or the main menu state
	gameStates GAME_STATE;

	//The Window the game uses
	GLFWwindow *gameWindow;

	//GLSL programs
	GLSLProgram *lightProgram;

	//Matricies for use in the shader
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 modelViewProjectionMatrix;

	//Shader uniform variables
	int uniform_MVP;
	int uniform_LightPos;
	int uniform_texture;
};