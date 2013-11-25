#include "Engine.h"

Player::Player(){}

Player::Player(OBJModel object){

	position.x = object.getPosX();
	position.y = object.getPosY();
	position.z = object.getPosZ();

	
	rotation.x = 0.f;
	rotation.y = 0.f;
	rotation.z= 0.f;

	
	playerObject = object;
	//playerHitBox = object.getHitBox();

	velocity = glm::vec3(0,0,0);
	maxVelY = 64;

	acceleration = glm::vec3(0,0,0);
	impactForce = glm::vec3(0,0,0);
	totalForce = glm::vec3(0,0,0);
	gravityForce = glm::vec3(0,-1000,0);
	moveForce = glm::vec3(0,0,0);
	resistanceForce = glm::vec3(0,0,0);
	jumpForce = glm::vec3(0,0,0);

	jumpCount = 0;
}

Player::~Player(){}

	//player input
	//TODO put speed things here
//void Player::increaseVelX(float vt){
//	velocity.x += vt;
//}
//void Player::decreaseVelX(float vt){	
//	velocity.x -=vt;
//}

void Player::increaseVel(glm::vec3 vt)
{
	velocity += vt;
	velocity = velocity;
}

void Player::stopVelX(){
	velocity.x = 0;
}

void Player::stopVelY(){
	velocity.y = 0;
}

//Update the player's position
void Player::update(Assets &assetList, int playerIDNum,float t)
{
	std::vector<collisionObjects> tempBoundBoxes;
	for(unsigned int i = 0; i < assetList.objects.size(); ++i)
	{
		tempBoundBoxes.push_back(assetList.objects[i].getHitBox());
	}

	impactForce = glm::vec3(0,0,0);

	bool hitAnything = false;
	int whatHit;

	

	//Loop through all of the collision boxes to see if we hit any of them.
	for(unsigned int i = 0; i < tempBoundBoxes.size(); ++i)
	{
		
		if(isBoxBoxColliding(this->getPosX() + velocity.x, this->getPosY() + velocity.y, this->getPosZ() + velocity.z,
							playerObject.getHitBox().getSize().x, playerObject.getHitBox().getSize().y, playerObject.getHitBox().getSize().z,
							tempBoundBoxes[i].getPos().x, tempBoundBoxes[i].getPos().y, tempBoundBoxes[i].getPos().z,
							tempBoundBoxes[i].getSize().x, tempBoundBoxes[i].getSize().y, tempBoundBoxes[i].getSize().z))
		{
			hitAnything = true;
			whatHit = i;
		}
	}

	//on collision, set position at border of collision and stop velocity
	if(hitAnything)
	{
		if(this->position.x < tempBoundBoxes[whatHit].getPos().x){
			this->position.x = tempBoundBoxes[whatHit].getPos().x - tempBoundBoxes[whatHit].getSize().x - 0.001f;
			impactForce = glm::vec3(-5,0,0);
		}else{
			this->position.x = tempBoundBoxes[whatHit].getPos().x + tempBoundBoxes[whatHit].getSize().x + 0.001f;
			impactForce = glm::vec3(5,0,0);
		}
		this->stopVelX();
		
	}
	//else continue on
	else if(!hitAnything)
	{
		this->updatePos(t);
	}


	if((this->getVelX() < 200.f)&&(this->getVelX() > 200.f)){
		this->stopVelX();
	}
	if(this->getPosY() < 0){
		position.y = 0;
		this->stopVelY();
		jumpCount = 0;
		gravityForce = glm::vec3(0,-1000,0);
	}

	//update object and hitbox
	playerObject.setPos(position.x,position.y,position.z);
	playerObject.getHitBox().setPos(position.x,position.y,position.z);;
}

void Player::updatePos(float t){
	
	if(velocity.y > maxVelY){
			jumpForce = glm::vec3(0,0,0);
	}
	if((position.y > 0)||((moveForce.x < 0)&&(velocity.x < 0))||((moveForce.x>0)&&(velocity.x > 0))){
		resistanceForce.x = -1 * velocity.x;
	} else {
		resistanceForce.x = -200 * velocity.x;
	}
	if((velocity.y > -1.f) && (velocity.y < 1.f)&&(jumpCount != 0)){
		gravityForce = glm::vec3(0,-5000,0);
	} else if (jumpCount != 0) {
		gravityForce += glm::vec3(0,-100,0);
	}

	totalForce = impactForce + gravityForce + moveForce + resistanceForce + jumpForce;
	acceleration = t*totalForce;
	velocity += t*acceleration;
	position += t*velocity + 0.5f*acceleration*t*t;
}

//draw player
void Player::draw(){
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	particlemanager.update(1.f);
	particlemanager.draw();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPushMatrix();

	//Bind the texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->getObject().getTex());           

	glTranslatef(position.x, position.y, position.z);
	//glTranslatef(this->playerObject.getHitBox().getPos().x, this->playerObject.getHitBox().getPos().y, this->playerObject.getHitBox().getPos().z);

	
	glBegin(GL_TRIANGLES);

	
	for(int j = 0; j < playerObject.getVerSize(); j++){
		
		glNormal3f( playerObject.getNormal(j).x,playerObject.getNormal(j).y,playerObject.getNormal(j).z);
		
		glTexCoord2f(playerObject.getUV(j).x,playerObject.getUV(j).y);
		
		glVertex3f( playerObject.getVertex(j).x,playerObject.getVertex(j).y,playerObject.getVertex(j).z);
	}
	
	glEnd();
	glPopMatrix();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_TEXTURE_2D);
}


//update actions
void Player::updateAction(int numAction){
	switch(numAction){
		//standard move left
		case 1:
			this->moveAction(numAction);
			break;
		//standard move right
		case 2:
			this->moveAction(numAction);
			break;
		//dash left
		case 3:
			break;
		//dash right
		case 4:
			break;
		//jump
		case 5:
			this->moveAction(numAction);
			break;
		default:
			moveForce = glm::vec3(0,0,0);
			break;
			//idle
	}
}

//movement + any collisions
void Player::moveAction(int numAction){
	if(numAction == 1){
		ParticleEmitter newemitter;
        Range tempRange[3];
        newemitter.setNumOfParticles(1000);
        newemitter.setLifeRange(Range(0,5));
        tempRange[0] = Range(position.x-1,position.x+1);
        tempRange[1] = Range(position.y-1,position.y+1);
        tempRange[2] = Range(position.z-1,position.z+1);
        newemitter.setPosRange(tempRange);
        
        tempRange[0] = Range(-5,0);
        tempRange[1] = Range(-1,1);
        tempRange[2] = Range(-2,2);
        newemitter.setVelRange(tempRange);

        tempRange[0] = Range(-1,0);
        tempRange[1] = Range(-1,2);
        tempRange[2] = Range(-2,2);
        newemitter.setAcelRange(tempRange);
        
        tempRange[0] = Range(0,2);
        tempRange[1] = Range(0,2);
        tempRange[2] = Range(0,1);
        newemitter.setSizeRange(tempRange);

        tempRange[0] = Range(240,256);
        tempRange[1] = Range(0,10);
        tempRange[2] = Range(0,10);
        newemitter.setColourRange(tempRange);

        newemitter.initialise();
        particlemanager.addEmmiter(newemitter);
		moveForce = glm::vec3(2000,0,0);
	} else if (numAction == 2){
		ParticleEmitter newemitter;
		Range tempRange[3];
		newemitter.setNumOfParticles(1000);
		newemitter.setLifeRange(Range(0,5));
		tempRange[0] = Range(position.x-1,position.x+1);
		tempRange[1] = Range(position.y-1,position.y+1);
		tempRange[2] = Range(position.z-1,position.z+1);
		newemitter.setPosRange(tempRange);
		
		tempRange[0] = Range(0,5);
		tempRange[1] = Range(-1,1);
		tempRange[2] = Range(-2,2);
		newemitter.setVelRange(tempRange);
		
		tempRange[0] = Range(0,1);
		tempRange[1] = Range(-1,2);
		tempRange[2] = Range(-2,2);
		newemitter.setAcelRange(tempRange);
		
		tempRange[0] = Range(0,2);
		tempRange[1] = Range(0,2);
		tempRange[2] = Range(0,1);
		newemitter.setSizeRange(tempRange);
		
		tempRange[0] = Range(240,256);
		tempRange[1] = Range(0,10);
		tempRange[2] = Range(0,10);
		newemitter.setColourRange(tempRange);
		
		newemitter.initialise();
		particlemanager.addEmmiter(newemitter);
		moveForce = glm::vec3(-2000,0,0);
	} else if (numAction == 5){
		if(jumpCount == 0){
			ParticleEmitter newemitter;
			
			Range tempRange[3];
			newemitter.setNumOfParticles(1000);
			newemitter.setLifeRange(Range(0,5));
			tempRange[0] = Range(position.x-1,position.x);
			tempRange[1] = Range(position.y-1,position.y);
			tempRange[2] = Range(position.z-1,position.z);
			newemitter.setPosRange(tempRange);
			
			tempRange[0] = Range(0,1);
			tempRange[1] = Range(-1,0);
			tempRange[2] = Range(0,1);
			newemitter.setVelRange(tempRange);
			
			tempRange[0] = Range(-1,1);
			tempRange[1] = Range(-3,0);
			tempRange[2] = Range(-1,1);
			newemitter.setAcelRange(tempRange);
			
			tempRange[0] = Range(0,2);
			tempRange[1] = Range(0,2);
			tempRange[2] = Range(0,1);
			newemitter.setSizeRange(tempRange);
			
			tempRange[0] = Range(240,256);
			tempRange[1] = Range(0,10);
			tempRange[2] = Range(0,10);
			newemitter.setColourRange(tempRange);

			newemitter.initialise();
			particlemanager.addEmmiter(newemitter);
			jumpForce = glm::vec3(0,100000,0);
			++jumpCount;
		}
	}
}





//getters and setters

	//object
OBJModel Player::getObject(){
	return playerObject;
}
	void setObject(OBJModel object);

	//position
	glm::vec3 Player::getPos(){return glm::vec3(position.x,position.y, position.z);}
	void setPos(glm::vec3 newposition);

	float Player::getPosX(){return position.x;}
	float Player::getPosY(){return position.y;}
	float Player::getPosZ(){return position.z;}

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

	float Player::getVelX(){
		return velocity.x;
	}
	float Player::getVelY(){
		return velocity.y;
	}
	float getVelZ();

	void setVelX();
	void setVelY();
	void setVelZ();
