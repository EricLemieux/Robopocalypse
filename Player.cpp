#include "Engine.h"

Player::Player(){}

Player::Player(OBJModel object){

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
}
void Player::decreaseVelX(float vt){	
	velocity.x -=vt;
}

void Player::stopVelX(){
	velocity.x = 0;
}

//Update the player's position
void Player::update(std::vector<collisionObjects> &tempBoundBoxes, int playerIDNum)
{
	if(velocity.x > 0.1)
		velocity.x -= 0.1;
	else if(velocity.x < -0.1)
		velocity.x += 0.1;
	else
		velocity.x = 0.0f;

	bool hitAnything = false;
	int whatHit;

	//Loop through all of the collision boxes to see if we hit any of them.
	for(int i = 0; i <= tempBoundBoxes.size()-1; i++)
	{
		if(i == playerIDNum)
			i++;
		if(isBoxBoxColliding(this->getPosX() + velocity.x, this->getPosY() + velocity.y, this->getPosZ() + velocity.z,
							tempBoundBoxes[playerIDNum].getSize().x, tempBoundBoxes[playerIDNum].getSize().y, tempBoundBoxes[playerIDNum].getSize().z,
							tempBoundBoxes[i].getPos().x, tempBoundBoxes[i].getPos().y, tempBoundBoxes[i].getPos().z,
							tempBoundBoxes[i].getSize().x, tempBoundBoxes[i].getSize().y, tempBoundBoxes[i].getSize().z))
		{
			hitAnything = true;
			whatHit = i;
		}
	}

	if(hitAnything)
	{
		if(this->position.x < tempBoundBoxes[whatHit].getPos().x)
			this->position.x = tempBoundBoxes[whatHit].getPos().x - tempBoundBoxes[whatHit].getSize().x - 0.001f;
		else
			this->position.x = tempBoundBoxes[whatHit].getPos().x + tempBoundBoxes[whatHit].getSize().x + 0.001f;
		this->stopVelX();
	}
	else if(!hitAnything)
	{
		position.x += velocity.x;
	}
}

//draw player
void Player::draw(){
	

	glPushMatrix();

	//Bind the texture
	glEnable(GL_TEXTURE_2D);
	//sf::Texture::bind(&texMap);
	glBindTexture(GL_TEXTURE_2D, this->getObject().getTex());

	//TODO remove and set actual max speed settings 
	//if(this->getVelX() > 0.1f){
	//		this->decreaseVelX(0.1f);
	//	} else if (this->getVelX() < -0.1f){
	//		this->increaseVelX(0.1f);
	//	} else {
	//		this->stopVelX();
	//	}

	//position.x += velocity.x;

	glTranslatef(position.x, position.y, position.z);

	
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
	sf::Vector3f Player::getPos(){return position;}
	void setPos(sf::Vector3f newposition);

	float Player::getPosX(){return position.x;}
	float Player::getPosY(){return position.y;}
	float Player::getPosZ(){return position.z;}

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
