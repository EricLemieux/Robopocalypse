#include "Engine.h"

Player::Player(){}

Player::Player(OBJModel object){

	position.x = object.getPosX();
	position.y = object.getPosY();
	position.z = object.getPosZ();
	prevpos = position;

	
	rotation.x = 0.f;
	rotation.y = 0.f;
	rotation.z= 0.f;

	
	playerObject = object;
	//playerHitBox = object.getHitBox();

	velocity = glm::vec3(0,0,0);
	maxVelY = 64;

	isMoving = 0;

	acceleration = glm::vec3(0,0,0);
	pushForce = glm::vec3(0,0,0);
	impactForce = glm::vec3(0,0,0);
	totalForce = glm::vec3(0,0,0);
	gravityForce = glm::vec3(0,-1000,0);
	moveForce = glm::vec3(0,0,0);
	resistanceForce = glm::vec3(0,0,0);
	jumpForce = glm::vec3(0,0,0);

	jumpCount = 0;

	//init attack collision boxes
	attackFist.setPos(glm::vec3(0,0,500));
	attackFist.setSize(glm::vec3(1,1,1));

	attackKick.setPos(glm::vec3(0,0,500));
	attackKick.setSize(glm::vec3(1,1,1));

	attackRange.setPos(glm::vec3(0,0,500));
	attackRange.setSize(glm::vec3(1,1,1));

	blockBox.setPos(glm::vec3(0,0,500));
	blockBox.setSize(object.getHitBox().getSize().x+1,object.getHitBox().getSize().y+1,object.getHitBox().getSize().z+1);

	attackStartPos = glm::vec3(0,0,500);
	
	isAttacking = 0;
	isKicking = 0;
	onCooldown = 0;
	dt = 0.f;
	tInterval = 0.f;
	
	isBlocking = 0;
	
	boosterCooldown = 0;
	jumpCooldown = 0;

	stunCooldown = 0;
	wasHit = 0;
	invincibleFrames = 0;

	faceDirection = 1;
	bodyTobody = 0;

	hitFloor	= false;
	hitWall		= false;

	health = 100;
	shield = 50;
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
void Player::update(Assets &assets, int playerIDNum,float t, Player &otherPlayer)
{
	//Check collisions against the other player
	checkOtherPlayer(otherPlayer);

	std::vector<collisionObjects> tempBoundBoxes;
	for(unsigned int i = 0; i < assets.objects.size(); ++i)
	{
		tempBoundBoxes.push_back(assets.objects[i].getHitBox());
	}
	floorPos = tempBoundBoxes[0].getPos().y;
	leftWallPos = tempBoundBoxes[1].getPos().x;
	rightWallPos = tempBoundBoxes[2].getPos().x;
	floorSize = tempBoundBoxes[0].getSize().y;
	leftWallSize = tempBoundBoxes[1].getSize().x;
	rightWallSize = tempBoundBoxes[2].getSize().x;

	bool hitAnything = false;
	int whatHit;

	
	


	if(onCooldown > 0){
		--onCooldown;
	}
	if(isBlocking > 0){
		--isBlocking;
		blockBox.setPos(position);
	} else {
		blockBox.setPos(0,0,500);
	}
	if(boosterCooldown > 0){
		--boosterCooldown;
	}
	if(jumpCooldown > 0){
		--jumpCooldown;
	}
	if(stunCooldown >  0){
		--stunCooldown;	
	}
	if(wasHit > 0){
		--wasHit;
	}
	if(invincibleFrames > 0){
		--invincibleFrames;
	}
	//Checking X values
	hitAnything = false;
	
	//Loop through all collision boxes to see if anything hit on the 
	for(unsigned int i = 0; i < tempBoundBoxes.size(); ++i)
	{
		if(isBoxBoxColliding(this->position + this->velocity * t, this->playerObject.getHitBox().getSize() * glm::vec3(1.0f, 0.0f, 0.0f), tempBoundBoxes[i].getPos(), tempBoundBoxes[i].getSize()))
		{
			if(velocity.x >10.0f || velocity.x < -10.0f)
			{
				hitWall = true;
			}
				
			whatHit = i;
			//velocity.x = 0.0f;
		}
	}
	//if the player hit anything
	if(hitAnything)
	{
		hitWall = true;
	}
	//if the player didnt hit anything
	else
	{
		bool hitBuffer = false;
		for(unsigned int i = 0; i < tempBoundBoxes.size(); ++i)
		{
			if(isBoxBoxColliding(this->playerObject.getHitBox().getPos() + this->velocity * t, this->playerObject.getHitBox().getSize() * glm::vec3(1.1f,0.0f,0.0f), tempBoundBoxes[i].getPos(), tempBoundBoxes[i].getSize()))
			{
				hitBuffer = true;
			}
		}
		if(hitBuffer)
		{
			hitWall = true;
		}
		else
		{
			hitWall = false;
		}
	}
	
	/////
	//Checking Y axis
	/////

	//Reset the checks for what i could have hit
	hitAnything = false;

	//Loop through all collision boxes to see if anything hit on the 
	for(unsigned int i = 0; i < tempBoundBoxes.size(); ++i)
	{
		if(isBoxBoxColliding(this->playerObject.getHitBox().getPos() + this->velocity * t, this->playerObject.getHitBox().getSize() * glm::vec3(0.0f, 1.0f, 0.0f), tempBoundBoxes[i].getPos(), tempBoundBoxes[i].getSize()))
		{
			hitAnything = true;
			whatHit = i;
		}
	}
	if(hitAnything)
	{
		hitFloor = true;
	}
	//check to see if the player is not inside of the geometry but still within the buffer.
	else if(!hitAnything)
	{
		bool hitBuffer = false;
		for(unsigned int i = 0; i < tempBoundBoxes.size(); ++i)
		{
			if(isBoxBoxColliding(this->playerObject.getHitBox().getPos() + this->velocity * t, this->playerObject.getHitBox().getSize() * glm::vec3(0.0f,1.1f,0.0f), tempBoundBoxes[i].getPos(), tempBoundBoxes[i].getSize()))
			{
				hitBuffer = true;
			}
		}
		if(hitBuffer)
		{
			hitFloor = true;
		}
		else
		{
			hitFloor = false;
		}
	}

	if(isAttacking == 6){
		if(!hitWall){
			if(dt<1){
				dt += tInterval;
				if(faceDirection == 1){
					velocity.x = 75.f;
					attackFist.setPos(position.x+8,position.y+3.5,position.z);
					attackStartPos = attackFist.getPos();
					attackFist.setPos(LERP(attackStartPos,glm::vec3(attackStartPos.x+10,attackStartPos.y,attackStartPos.z),dt));
				} else if (faceDirection == -1){
					velocity.x = -75.f;
					attackFist.setPos(position.x-8,position.y+3.5,position.z);
					attackStartPos = attackFist.getPos();
					attackFist.setPos(LERP(attackStartPos,glm::vec3(attackStartPos.x-10,attackStartPos.y,attackStartPos.z),dt));
				}
			} else {
				isAttacking = 0;
				dt = 0.f;
				tInterval = 0.f;
				attackFist.setPos(0,0,500);
			}
		} else {
			isAttacking = 0;
			dt = 0.f;
			tInterval = 0.f;
			attackFist.setPos(0,0,500);
		}
	}
	if(isAttacking == 8){
		if(!hitWall){
			if(dt<1){
				dt += tInterval;
				if(faceDirection == 1){
					velocity.x = 75.f;
					attackKick.setPos(position.x+8,position.y-3.5,position.z);
					attackStartPos = attackKick.getPos();
					attackKick.setPos(LERP(attackStartPos,glm::vec3(attackStartPos.x+10,attackStartPos.y+5,attackStartPos.z),dt));
				} else if (faceDirection == -1){
					velocity.x = -75.f;
					attackKick.setPos(position.x-8,position.y-3.5,position.z);
					attackStartPos = attackKick.getPos();
					attackKick.setPos(LERP(attackStartPos,glm::vec3(attackStartPos.x-10,attackStartPos.y+5,attackStartPos.z),dt));
				}
				if(hitFloor){
					isKicking += 1;
				} else {
					if(isKicking == 1)
					velocity.y=-75.f;
					
					
				}
			} else {
				isAttacking = 0;
				dt = 0.f;
				tInterval = 0.f;
				attackKick.setPos(0,0,500);
				isKicking = 0;
			}
		} else {
			isAttacking = 0;
			dt = 0.f;
			tInterval = 0.f;
			attackKick.setPos(0,0,500);
			isKicking = 0;
		}
	}

	this->updatePos(t);

	//if(this->getPosY() < 0){
 	//	position.y = 0;
	//	this->stopVelY();
	//	jumpCount = 0;
	//	gravityForce = glm::vec3(0,-1000,0);
	//}

	//update object and hitbox
	playerObject.setPos(position.x,position.y,position.z);
	playerObject.getHitBox().setPos(position.x,position.y,position.z);
}

void Player::checkOtherPlayer(Player &otherPlayer)
{
	//check to see which way should be facing
	if(isAttacking == 0){
		if(position.x < otherPlayer.getPosX()){
			faceDirection = 1;
		} else if(position.x > otherPlayer.getPosX()) {
			faceDirection = -1;
		} else {
			faceDirection = 0;
		}
	}
	bodyTobody = 0;
	if(invincibleFrames == 0){
		//Check to see if the player is being hit with a punch
		if(isBoxBoxColliding(playerObject.getHitBox().getPos(),playerObject.getHitBox().getSize(), otherPlayer.attackFist.getPos(), otherPlayer.attackFist.getSize()))
		{
			//Player is getting punched
			//TODO
			if((isBlocking > 0) && (shield != 0)){
				if(shield > 0)
					shield -= 10;
			} else {
				if(health > 0)
					health -= 10;
			}
			stunCooldown = 10;
			invincibleFrames = 10;
			wasHit = 2;
			if(faceDirection == 1){
				impactForce = glm::vec3(-50000,0,0);
			} else if(faceDirection == -1) {
				impactForce = glm::vec3(50000,0,0);
			}
		}

		//Check to see if the player is being hit with a kick
		if(isBoxBoxColliding(playerObject.getHitBox().getPos(),playerObject.getHitBox().getSize(), otherPlayer.attackKick.getPos(), otherPlayer.attackKick.getSize()))
		{
			//Player is getting kicked
			//TODO
			if((isBlocking > 0)&&(shield != 0)){
				if(shield > 10)
					shield -= 20;
				else if (shield > 0)
					shield-=10;
			} else {
				if(health > 10)
					health -= 20;
				else if (health > 0)
					health-=10;
			}
			jumpCount = 1;
			stunCooldown = 30;
			invincibleFrames = 20;
			wasHit = 2;
			if(faceDirection == 1){
				impactForce = glm::vec3(-40000,80000,0);
			} else if(faceDirection == -1) {
				impactForce = glm::vec3(40000,80000,0);
			}
		}

		//Check to see if the player is being hit with a ranged attack
		if(isBoxBoxColliding(playerObject.getHitBox().getPos(),playerObject.getHitBox().getSize(), otherPlayer.attackRange.getPos(), otherPlayer.attackRange.getSize()))
		{
			//Player is getting hit with a range attack
			//TODO
		}
	}
	//if player is colliding with another player
	if(isBoxBoxColliding(playerObject.getHitBox().getPos(),playerObject.getHitBox().getSize(),otherPlayer.getObject().getHitBox().getPos(),otherPlayer.getObject().getHitBox().getSize())){
		//wasHit = 2;
		//pushForce = glm::vec3(otherPlayer.getVelocity().x*0.03,otherPlayer.getVelocity().y*0.03,otherPlayer.getVelocity().z*0.03);

		//bodyTobody = 1;
		//position = prevpos;
		
		//if((faceDirection == 1)&&(position.y < otherPlayer.getPosY()+ (otherPlayer.getObject().getHitBox().getSize().y/2))&&(position.y > otherPlayer.getPosY()-(otherPlayer.getObject().getHitBox().getSize().y/2))){
		//	position.x = otherPlayer.getObject().getHitBox().getPos().x- (otherPlayer.getObject().getHitBox().getSize().x/2)- (playerObject.getHitBox().getSize().x/2);
		//	bodyTobody = 2;
		//	pushForce = glm::vec3(0.5f*otherPlayer.getVelX()*otherPlayer.getVelX(),0,0);
		//} else if ((faceDirection == -1)&&(position.y < otherPlayer.getPosY()+(otherPlayer.getObject().getHitBox().getSize().y/2))&&(position.y > otherPlayer.getPosY()-(otherPlayer.getObject().getHitBox().getSize().y/2))){
		//	position.x = otherPlayer.getObject().getHitBox().getPos().x+ (otherPlayer.getObject().getHitBox().getSize().x/2)+ (playerObject.getHitBox().getSize().x/2);
		//	bodyTobody = 2;
		//	pushForce = glm::vec3(0.5f*otherPlayer.getVelX()*otherPlayer.getVelX(),0,0);
		//} 
		
		//else if (position.y-playerObject.getHitBox().getSize().y/2 < otherPlayer.getPosY()+otherPlayer.getObject().getHitBox().getSize().y/2){
		//	position.y = otherPlayer.getObject().getHitBox().getPos().y- (otherPlayer.getObject().getHitBox().getSize().y/2)- (playerObject.getHitBox().getSize().y/2);
		//	wasHit = 2;
		//	impactForce = glm::vec3(0,-0.5f*otherPlayer.getVelY()*otherPlayer.getVelY(),0);
		//} else if (position.y+playerObject.getHitBox().getSize().y/2 > otherPlayer.getPosY()-otherPlayer.getObject().getHitBox().getSize().y/2){
		//	position.y = otherPlayer.getObject().getHitBox().getPos().y+ (otherPlayer.getObject().getHitBox().getSize().y/2)+ (playerObject.getHitBox().getSize().y/2);
		//	wasHit = 2;
		//	impactForce = glm::vec3(0,0.5f*otherPlayer.getVelY()*otherPlayer.getVelY() + (-gravityForce.y),0);
		//}
	}
}

void Player::updatePos(float t){
	
	if(velocity.y > maxVelY){
			jumpForce = glm::vec3(0,0,0);
	}
	if((!hitFloor)||((moveForce.x < 0)&&(velocity.x < 0))||((moveForce.x>0)&&(velocity.x > 0))){
		resistanceForce.x = -1 * velocity.x;
	} else {
		resistanceForce.x = -200 * velocity.x;
	}

	if(!hitFloor)
	{
		gravityForce.y = -5000.0f;
	} else {
		gravityForce.y = 0.f;
	}
	if((velocity.y > -1.f) && (velocity.y < 1.f)&&(jumpCount != 0)){
		gravityForce = glm::vec3(0,-5000,0);
	} else if (jumpCount != 0) {
		gravityForce += glm::vec3(0,-100,0);
	}
	

	if(isMoving == 0){
	    moveForce = glm::vec3(0,0,0);
	}
	
	if(isBlocking > 0){
	    jumpForce = jumpForce * 0.01f;
	    moveForce = moveForce*0.01f;
	    impactForce = impactForce*0.1f;
	}

	if(wasHit == 0){
		impactForce = glm::vec3(0,0,0);
	}

	if(bodyTobody == 0){
		pushForce = glm::vec3(0,0,0);
	}

	totalForce = impactForce + gravityForce + moveForce + resistanceForce + jumpForce + pushForce;
	
	if(!hitFloor)
	{
		jumpCount = 1;
		acceleration.y = t*totalForce.y;
		velocity.y += t*acceleration.y;
		position.y += t*velocity.y + 0.5f*acceleration.y*t*t;
	}
	else if(hitFloor)
	{
		
		//position.y		= position.y - t*velocity.y - 0.5f*acceleration.y*t*t;
		//hack, gross
		position.y = floorPos + (floorSize/2) + (playerObject.getHitBox().getSize().y/2);

		gravityForce.y	= 0.0f;
		//totalForce.y	= 0.0f;
		acceleration.y	= 0.0f;
		if(isKicking == 1)
			isKicking = 0;
		if(isKicking == 0)
			velocity.y	= 0.0f;
		else if (isKicking > 1)
			velocity.y = 75.f;
		if(jumpCount > 0){
			acceleration.y = t*totalForce.y;
			velocity.y += t*acceleration.y;
			position.y += t*velocity.y + 0.5f*acceleration.y*t*t;
		}
		
		jumpCount		= 0;
	}

	if(!hitWall)
	{
		acceleration.x = t*totalForce.x;
		velocity.x += t*acceleration.x;
		position.x += t*velocity.x + 0.5f*acceleration.x*t*t;
	}
	else if(hitWall)
	{
		position.x = position.x - t*velocity.x - 0.5f*acceleration.x*t*t;
		//hackhackhack
		//if(position.x < 0){
		//	position.x = leftWallPos+(leftWallSize/2)+(playerObject.getHitBox().getSize().x/2);
		//} else {
		//	position.x = rightWallPos-(rightWallSize/2)-(playerObject.getHitBox().getSize().x/2);
		//}

		totalForce.x	= impactForce.x = gravityForce.x = moveForce.x = resistanceForce.x = jumpForce.x = 0.0f;
		//totalForce.x = 0.0f;
		acceleration.x	= 0.0f;
		velocity.x		= 0.0f;
	}
}

//draw player
void Player::draw(){
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	particlemanager.update(1.f);
	particlemanager.draw();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
	if(stunCooldown == 0){
		switch(numAction){
			//standard move left
			case 1:
				this->moveAction(numAction);
				isMoving = numAction;
				break;
			//standard move right
			case 2:
				this->moveAction(numAction);
				isMoving = numAction;
				break;
			//dash left
			case 3:
				if(boosterCooldown == 0){
				     this->moveAction(numAction);
				     isMoving = numAction;
				 } else {
				     isMoving = 0;
				 }
				break;
			//dash right
			case 4:
				if(boosterCooldown == 0){
			    this->moveAction(numAction);
			    isMoving = numAction;
			} else {
			    isMoving = 0;
			}
				break;
			//jump
			case 5:
				if(jumpCooldown == 0){
			        this->moveAction(numAction);
			        isMoving = 0;
			    }
			    break;
			//attack pawnch
			case 6:
			    if(onCooldown == 0){
			        this->attackAction(numAction);
			        isAttacking = numAction;
			        isMoving = 0;
			    }
			    break;
			//block
			case 7:
			    if(onCooldown == 0){
			        this->blockAction(numAction);
			        isMoving = 0;
			    }
				break;
				//kick
			case 8:
				if(onCooldown == 0){
					this->attackAction(numAction);
					isAttacking = numAction;
					isMoving = 0;
				}
			default:
				isMoving = 0;
				break;
				//idle
		}
	}
}

//movement + any collisions
void Player::moveAction(int numAction){
	if(numAction == 1){
		 moveForce = glm::vec3(2000,0,0);
	} else if (numAction == 2){
	    moveForce = glm::vec3(-2000,0,0);
	} else if (numAction == 3){
	    boosterCooldown = 60;
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
		moveForce = glm::vec3(150000,0,0);
	} else if (numAction == 4){
		boosterCooldown = 60;
		ParticleEmitter newemitter;
		Range tempRange[3];
		newemitter.setNumOfParticles(1000);
		newemitter.setLifeRange(Range(0,5));
		tempRange[0] = Range(position.x-1,position.x+1);
		tempRange[1] = Range(position.y-1,position.y+1);
		tempRange[2] = Range(position.z-1,position.z+1);
		newemitter.setPosRange(tempRange);
		
		tempRange[0] = Range(1,6);
		tempRange[1] = Range(-1,1);
		tempRange[2] = Range(-2,2);
		newemitter.setVelRange(tempRange);
		
		tempRange[0] = Range(1,2);
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
		moveForce = glm::vec3(-150000,0,0);
	} else if (numAction == 5){
		if(jumpCount == 0){
			jumpCooldown = 60;

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
			jumpForce = glm::vec3(0,120000,0);
			++jumpCount;
		}
	}
}

void Player::attackAction(int numAction){
    isAttacking = numAction;
    if(numAction == 6){
        tInterval = 0.15f;
        attackFist.setPos(position.x+8,position.y+3.5,position.z);
        attackStartPos = attackFist.getPos();
        onCooldown = 20;
    } else if(numAction == 8){
        tInterval = 0.05f;
        attackKick.setPos(position.x+8,position.y-3.5,position.z);
        attackStartPos = attackKick.getPos();
        onCooldown = 40;
		isKicking = 1;

    }
}

void Player::blockAction(int numAction){
    isBlocking = 20;
    onCooldown = 30;
	blockBox.setPos(position);
}

void Player::Death()
{
	ParticleEmitter deathEmitter;

	deathEmitter.setNumOfParticles(20);

	Range lifeRange;
	lifeRange = Range(0,5);
	deathEmitter.setLifeRange(lifeRange);

	Range posRange[3];
	posRange[0] = Range(position.x-1,position.x+1);
	posRange[1] = Range(position.y-1,position.y+1);
	posRange[2] = Range(position.z-1,position.z+1);
	deathEmitter.setPosRange(posRange);

	Range clrRange[3];
	clrRange[0] = Range(200,250);
	clrRange[1] = Range(0,250);
	clrRange[2] = Range(0,250);
	deathEmitter.setColourRange(clrRange);

	Range aclRange[3];
	aclRange[0] = Range(-1,1);
	aclRange[1] = Range(-1,1);
	aclRange[2] = Range(-1,1);
	deathEmitter.setAcelRange(aclRange);

	deathEmitter.initialise();

	particlemanager.addEmmiter(deathEmitter);
}

//getters and setters

	//object
OBJModel Player::getObject(){
	return playerObject;
}
	void setObject(OBJModel object);

	//position
	glm::vec3 Player::getPos(){return glm::vec3(position.x,position.y, position.z);}
	void Player::setPos(glm::vec3 newposition){
		position = newposition;
	}

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
	glm::vec3 Player::getVelocity()
	{
		return velocity;
	}
	void setVelocity(glm::vec3 newvelocity);

	float Player::getVelX(){
		return velocity.x;
	}
	float Player::getVelY(){
		return velocity.y;
	}
	float Player::getVelZ()
	{
		return velocity.z;
	}

	void setVelX();
	void setVelY();
	void setVelZ();

	int Player::getHealth(){return health;}
	int Player::getShield(){return shield;}

	void setHealth();
	void setShield();

	glm::vec3 Player::getTotalForce(){
		return totalForce;
	}