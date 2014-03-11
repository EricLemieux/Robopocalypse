#pragma once

#include "PlayerActions.h"
#include "GameObject.h"
#include "SceneGraph.h"
#include "Sound.h"

class Player : public GameObject
{
private:
	glm::vec3 pos;
	glm::vec3 vel;

	Node *sceneGraphNode;

	int hp, maxHP;
	int sp, maxSP;

	std::vector<CollisionBox> hitboxList;

	//action list
	Actions prevAction;
	Actions currentAction;
	Actions nextAction;

	//actions
	PlayerActions playerAction;

	//-1 left, 1 right, 0 default
	int isFacing;
	//0 no, 1 yes
	int onGround;

	//1/framerate for the purposes of tracking time
	float freq;

	//action timer
	int actionTimer;

	//collision booleans TODO

	//blast shield stun cooldown
	int blastStunCooldown;
	
	//temp timer to replace controller
	int tempTimer;

	//max distance between players
	float maxDistBetweenPlayers;

	//prevents multiple hits from one attack
	int hasBeenHit;


public:
	Player();
	~Player();
	void update(Player *otherPlayer, playerSFX &sfx);
	void draw();

	Actions controllerInput(Actions action);

	void cycleActions();

	std::vector<CollisionBox> GetCollisionBoxes(void);

	inline int GetHP(void){ return hp; }
	inline int GetSP(void){ return sp; }

	inline int GetMaxHP(void) { return maxHP; }
	inline int GetMaxSP(void) { return maxSP; }

	glm::vec3 getPos();

	void soundCheck(playerSFX &sfx);

	 //for affecting other player on blocking melee attack
	 void setNextAction(Actions newAction);
	 int getOnGround();
};