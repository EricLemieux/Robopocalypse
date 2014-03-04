#pragma once

#include "PlayerActions.h"
#include "GameObject.h"
#include "SceneGraph.h"

class Player : public GameObject
{
private:
	glm::vec3 pos;
	glm::vec3 vel;

	Node *sceneGraphNode;

	int hp;
	int sp;

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

public:
	Player();
	~Player();
	void update(Player otherPlayer);
	void draw();

	Actions controllerInput(Actions action);

	void activateHitBox(Hitboxes box);
	void deactivateHitbox(Hitboxes box);

	void cycleActions();

	std::vector<CollisionBox> GetCollisionBoxes(void);
};