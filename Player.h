#ifndef PLAYER_H
#define PLAYER_H

class Player{
public:
	Player();
	Player(OBJModel object);
	~Player();

	//player input
	//TODO put speed things here
	//void increaseVelX(float vt);
	//void decreaseVelX(float vt);
	void increaseVel(glm::vec3 vt);
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
	//glm::vec3 position;
	glm::vec3 position;
	glm::vec3 rotation;
	//glm::vec3 velocity;
	glm::vec3 velocity;
	
};



#endif