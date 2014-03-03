#ifndef PLAYERACTIONS_H
#define PLAYERACTIONS_H

#include "AnimationMath.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <iostream>
#include <vector>

#include "CollisionBox.h"


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

class PlayerActions{
public:

	PlayerActions();
	~PlayerActions();

	//movement does not return IDLE ever, gets overridden in player
	Actions moveLeftAction(int &t, glm::vec3 &vel);
	Actions moveRightAction(int &t, glm::vec3 &vel);
	
	//dash cancel out of move
	Actions dashLeftAction(glm::vec3 &vel, int &sp);
	Actions dashRightAction(glm::vec3 &vel, int &sp);
	
	//jump cancel out of move or dash
	Actions jumpAction(glm::vec3 &vel, int onGround);
	
	//attack out of move or dash or jump
	Actions punchAction(int &t, glm::vec3 &vel, int facing,					std::vector<CollisionBox> &hitboxList, int onGround);
	Actions kickAction(int &t,	glm::vec3 &vel, int facing,	int onGround,	std::vector<CollisionBox> &hitboxList);
	Actions laserAction(int &t, glm::vec3 &vel, int facing,					std::vector<CollisionBox> &hitboxList);
	Actions blastAction(int &t, glm::vec3 &vel,								std::vector<CollisionBox> &hitboxList);
	
	//block out of everything
	Actions blockAction(int &t, glm::vec3 &vel, std::vector<CollisionBox> &hitboxList, int &sp);
	
	//stagger out of everything except block
	Actions staggerGAction(int &t, glm::vec3 &vel, int facing, std::vector<CollisionBox> &hitboxList);
	Actions staggerAAction(int &t, glm::vec3 &vel, int facing, std::vector<CollisionBox> &hitboxList);
	
	Actions idleAction(int &t, std::vector<CollisionBox> &hitboxList);
	
	void activateHitbox(CollisionBox &box);
	void deactivateHitbox(CollisionBox &box);
	
	//use this to clear all hitboxes except body
	void deactivateAllHitbox(std::vector<CollisionBox> &hitboxList);

private:
	int kickStart;//0 started on ground, 1 started in air
};
#endif