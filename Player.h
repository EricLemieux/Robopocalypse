#ifndef PLAYER_H
#define PLAYER_H
#include "Engine.h"

class Player{
public:
	Player();
	Player(OBJModel object);
	~Player();

	//player input
	//TODO put speed things here
	void increaseVelX(float vt);
	void decreaseVelX(float vt);
	void stopVelX();

	//Update the players current position
	//also check for collisions here.
	void update(std::vector<collisionObjects> &tempBoundBoxes, int playerIDNum);

	//draw player
	void draw();

//getters and setters

	//object
	OBJModel getObject();
	void setObject(OBJModel object);

	//position
	sf::Vector3f getPos();
	void setPos(sf::Vector3f newposition);

	float getPosX();
	float getPosY();
	float getPosZ();

	void setPosX(float nxPos);
	void setPosY(float nyPos);
	void setPosZ(float nzPos);

	//rotation
	sf::Vector3f getRotation();
	void setRotation(sf::Vector3f newrotation);

	float getRotX(float nxRot);
	float getRotY(float nyRot);
	float getRotZ(float nzRot);

	void setRotX();
	void setRotY();
	void setRotZ();

	//velocity
	sf::Vector3f getVelocity();
	void setVelocity(sf::Vector3f newvelocity);

	float getVelX();
	float getVelY();
	float getVelZ();

	void setVelX();
	void setVelY();
	void setVelZ();




private:
	OBJModel playerObject;
	sf::Vector3f position;
	sf::Vector3f rotation;
	sf::Vector3f velocity;
	
};



#endif