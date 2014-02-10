#pragma once

//////////
//INCLUDES
//////////
#include <glm\glm.hpp>

//////////
//GAME OBJECT CLASS
//
//This is the basic class that is used for everyevery object used in the game
//Some uses include: cameras, lights, players, etc..
//////////
class GameObject
{
public:
	//////////
	//CONSTRUCTORS
	//////////
	GameObject();
	~GameObject();

private:
	//////////
	//DATA
	//////////

	//Position
	glm::mat4 position;
	glm::mat4 rotation;
	glm::mat4 scale;
};

