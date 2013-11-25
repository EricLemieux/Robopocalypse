#ifndef PLAYER_H
#define PLAYER_H

class Player{
public:
	Player();
	Player(OBJModel object);
	~Player();

	
	//temporary, delete soon
	void increaseVel(glm::vec3 vt);
	//kill velocity
	void stopVelX();
	void stopVelY();
	
	//Update the players current position
	//also check for collisions here.
	void update(Assets &assetList, int playerIDNum,float t);
	void updatePos(float t);

	//*********************************
	//these should be called in UserInput.cpp
	//player input decides what action happens
	void updateAction(int numAction);

	//update movement vars + any collisions
	void moveAction(int numAction);
	//attack choice, play animation + move if necessary, 
	void attackAction(int numAction);
	//dodge, invincibility frames, animate, move, block input for set time
	void dodgeAction(int numAction);
	//block, reduce speed, block input for set time, set blocking status, animate
	void blockAction(int numAction);
	//***********************************

	//draw player
	void draw();

	//getters and setters

	//object
	OBJModel getObject();
	void setObject(OBJModel object);

	//position
	glm::vec3 getPos();
	void setPos(glm::vec3 newposition);

	float getPosX();
	float getPosY();
	float getPosZ();

	void setPosX(float nxPos);
	void setPosY(float nyPos);
	void setPosZ(float nzPos);

	//rotation
	glm::vec3 getRotation();
	void setRotation(glm::vec3 newrotation);

	float getRotX(float nxRot);
	float getRotY(float nyRot);
	float getRotZ(float nzRot);

	void setRotX();
	void setRotY();
	void setRotZ();

	//velocity
	glm::vec3 getVelocity();
	void setVelocity(glm::vec3 newvelocity);

	float getVelX();
	float getVelY();
	float getVelZ();

	void setVelX();
	void setVelY();
	void setVelZ();




private:
	OBJModel playerObject;
	//collisionObjects playerHitBox;
	int input;

	//glm::vec3 position;
	glm::vec3 position;
	glm::vec3 rotation;
	//glm::vec3 velocity;
	glm::vec3 velocity;
	float maxVelY;
	glm::vec3 acceleration;
	glm::vec3 totalForce;
	//force from collision with another player/attack
	glm::vec3 impactForce;
	//gravity
	glm::vec3 gravityForce;
	glm::vec3 moveForce;
	glm::vec3 resistanceForce;
	glm::vec3 jumpForce;

	//check for whether player input is accepted
	int onCooldown;
	//track current action
	int currentAction;
	//tracking for double jumps
	int jumpCount;

	//particle manager
	ParticleManager particlemanager;

};



#endif