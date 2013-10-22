#include "Engine.h"

Player::Player(){}

Player::Player(OBJModel object){

	sinceLastInput = 0;

	xt = 0.f;

	position.x = object.getPosX();
	position.y = object.getPosY();
	position.z = object.getPosZ();

	rotation.x = 0.f;
	rotation.y = 0.f;
	rotation.z= 0.f;

	velocity.x = 0.f;
	velocity.y = 0.f;
	velocity.z = 0.f;

	playerObject = object;
}

Player::~Player(){}

	//player input
	//TODO put speed things here
void Player::increaseVelX(float vt){
	velocity.x += vt;
	sinceLastInput = 0;
}
void Player::decreaseVelX(float vt){
	velocity.x -=vt;
	sinceLastInput = 0;
}

void Player::stopVelX(){
	velocity.x = 0;
}

	//draw player
void Player::draw(){
	glPushMatrix();

	if(this->getVelX() > 0.1f){
			this->decreaseVelX(0.1f);
		} else if (this->getVelX() < -0.1f){
			this->increaseVelX(0.1f);
		} else {
			this->stopVelX();
		}

	xt += velocity.x;

	glTranslatef(position.x+xt, position.y, position.z);

	
	glBegin(GL_TRIANGLES);

	
	for(int j = 0; j < playerObject.getVerSize(); j++){
		
		glNormal3f( playerObject.getNormal(j).x,playerObject.getNormal(j).y,playerObject.getNormal(j).z);
		
		glTexCoord2f(playerObject.getUV(j).x,playerObject.getUV(j).y);
		
		glVertex3f( playerObject.getVertex(j).x,playerObject.getVertex(j).y,playerObject.getVertex(j).z);
	}
	
	glEnd();
	glPopMatrix();
}

//getters and setters

	//object
OBJModel Player::getObject(){
	return playerObject;
}
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

	float Player::getVelX(){
		return velocity.x;
	}
	float getVelY();
	float getVelZ();

	void setVelX();
	void setVelY();
	void setVelZ();
