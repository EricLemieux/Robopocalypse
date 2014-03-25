#pragma once

//////////
//INCLUDES
//////////
#include <GL\glew.h>

#include "SceneGraph.h"
#include "GameObject.h"
#include "GLSL.h"
#include "Assets.h"
#include "Player.h"
#include "Camera.h"
#include "Menu.h"
#include "Sound.h"
#include "FrameBuffer.h"
#include "Light.h"

#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <glm\gtx\euler_angles.hpp>

#include <iostream>

#define GLFW_JOYSTICK_1 0
#define GLFW_JOYSTICK_2 1

#define MAX_BONE_SIZE 64

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

	//Initializes for gameplay
	void initGameplay(void);

	//Initializes for the menu
	void initMainMenu(void);

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

	inline Menu* GetMenu(void){ return gameMenu; }


	//////////
	//OTHERS
	//////////

	//Update the game
	void Update(void);

	//Get the players separate inputs
	void playerInput(void);

	//Render the game
	void Render(void);
	void RenderHUD(void);

	//Update the variables for the shaders before rendering the Game Object
	void PreRender(GameObject* object);
	void PreRender(GameObject* object, Light* light);
	void PreRender(std::vector<CollisionBox> hitboxes);

private:

	//////////
	//DATA
	//////////

	unsigned int windowWidth;
	unsigned int windowHeight;

	//Scene Graph nodes
	Node *world;
	Node *sceneGraph;
	
	//Game Objects
	Player *player1, *player2;

	Light *mainLight;

	Camera *mainCamera;

	Assets BackgroundObjects;

	//Stores if the game should be running
	bool isRunning;

	//Stores the current state of the game
	//Such as the gameplay state or the main menu state
	gameStates GAME_STATE;

	//The Window the game uses
	GLFWwindow *gameWindow;

	//GLSL programs
	GLSLProgram *diffuseProgram;
	GLSLProgram *meshSkinProgram;
	GLSLProgram *lightProgram;
	GLSLProgram *HUDProgram;
	GLSLProgram *OutlineProgram;

	//Matricies for use in the shader
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 modelViewProjectionMatrix;

	Menu *gameMenu;

	//Texture for the HUD
	GLuint HUDBackgroundHandle;
	GLuint HUDBarRedHandle, HUDBarBlueHandle;

	//Texture handle for the qMap used in toon shading
	GLuint qMap_handle;

	//Game Objects for the HUD and its components
	GameObject *HUD;
	GameObject *HUDBars[4];

	//Shader uniform variables
	unsigned int uniform_MVP;
	unsigned int uniform_texture;
	unsigned int uniform_normalMap;

	unsigned int uniform_meshSkin_MVP;
	unsigned int uniform_meshSkin_texture;
	unsigned int uniform_meshSkin_normalMap;
	unsigned int uniform_meshSkin_boneMat;

	unsigned int uniform_Light_LightPos;
	unsigned int uniform_Light_LightColour;
	unsigned int uniform_Light_qMap;

	unsigned int uniform_HUD_MVP;
	unsigned int uniform_HUD_texture;
	unsigned int uniform_HUD_FaceDirection;

	unsigned int uniform_outline_MVP;
	unsigned int uniform_outline_scene;

	//sound
	SoundWrap soundSystem;

	playerSFX pl1SFX;
	playerSFX pl2SFX;
	menuSFX mSFX;

	//Full screen quad used for drawing the frame buffer object to.
	VertexBuffer *fullScreenQuad;

	//Frame buffers
	FrameBuffer *firstPass;
};