#ifndef PLAYERACTIONS_H
#define PLAYERACTIONS_H

#include "AnimationMath.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <iostream>
#include <vector>


enum Actions{
	MOVE_LEFT = 0,
	MOVE_RIGHT,
	DASH_LEFT,
	DASH_RIGHT,
	JUMP,
	PUNCH,
	KICK,
	LASER,
	BLAST,
	BLOCK,
	STAGGER_G,
	STAGGER_A,
	IDLE,

};

enum Hitboxes{
	BODYBOX = 0,
	PUNCHBOX,
	KICKBOX,
	LASERBOX,
	BLASTBOX
};

struct Hitbox{
	glm::vec3 pos;
	//0 false, 1 true
	int isActive;
};

class PlayerActions{
public:

	PlayerActions();
	~PlayerActions();

	//movement does not return IDLE ever, gets overridden in player
	Actions moveLeftAction(int &t, glm::vec3 &vel);
	Actions moveRightAction(int &t, glm::vec3 &vel);
	
	//dash cancel out of move
	Actions dashLeftAction(glm::vec3 &vel);
	Actions dashRightAction(glm::vec3 &vel);
	
	//jump cancel out of move or dash
	Actions jumpAction(glm::vec3 &vel);
	
	//attack out of move or dash or jump
	Actions punchAction(int &t, glm::vec3 &vel, int facing		     , std::vector<Hitbox> &hitboxList);
	Actions kickAction(int &t , glm::vec3 &vel, int facing, int inAir, std::vector<Hitbox> &hitboxList);
	Actions laserAction(int &t, glm::vec3 &vel, int facing           , std::vector<Hitbox> &hitboxList);
	Actions blastAction(int &t, glm::vec3 &vel						 , std::vector<Hitbox> &hitboxList);
	
	//block out of everything
	Actions blockAction(int &t, glm::vec3 &vel, std::vector<Hitbox> &hitboxList, int &sp);
	
	//stagger out of everything except block
	Actions staggerGAction(int &t, glm::vec3 &vel, int facing, std::vector<Hitbox> &hitboxList);
	Actions staggerAAction(int &t, glm::vec3 &vel, int facing, std::vector<Hitbox> &hitboxList);
	
	Actions idleAction(int &t, std::vector<Hitbox> &hitboxList);
	
	void activateHitbox(Hitbox &box);
	void deactivateHitbox(Hitbox &box);
	
	//use this to clear all hitboxes except body
	void deactivateAllHitbox(std::vector<Hitbox> &hitboxList);

};
#endif