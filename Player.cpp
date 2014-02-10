#include "Engine.h"

Player::Player(){}

Player::Player(OBJModel object){

	//position of player
	position = object.getPos();
	prevpos = position;

	//orientation of player
	rotation.x = 0.f;
	rotation.y = 0.f;
	rotation.z= 0.f;

	
	playerObject = object;

	playerMorphs.objects.clear();
	playerMorphs.LoadAssets("assetsPlayerMorphs.txt");
	
	//set what animations are available
	Animations = new PlayerAnimation(playerObject, playerMorphs);

	//Idle
	Animations->createAnimation(0,1);
	//Punch
	Animations->createAnimation(2,4);
	//Kick
	Animations->createAnimation(5,7);
	//JumpKick
	Animations->createAnimation(8,9);
	//Block
	Animations->createAnimation(10,11);
	//Dash
	Animations->createAnimation(12,13);
	
	Animations->setAnimation(0);

	
	velocity	= glm::vec3(0,0,0);
	maxVelY		= 64;

	isMoving = 0;

	acceleration	= glm::vec3(0,0,0);
	pushForce		= glm::vec3(0,0,0);
	impactForce		= glm::vec3(0,0,0);
	totalForce		= glm::vec3(0,0,0);
	gravityForce	= glm::vec3(0,-1000,0);
	moveForce		= glm::vec3(0,0,0);
	resistanceForce = glm::vec3(0,0,0);
	jumpForce		= glm::vec3(0,0,0);

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
	
	//cooldowns
	isMoving		= 0;
	isDashing		= 0;

	isAttacking		= 0;
	isKicking		= 0;
	onCooldown		= 0;
	rangeCooldown	= 0;
	dt				= 0.f;
	rdt				= 0.f;
	tInterval		= 0.f;
	rtInterval		= 0.f;
	
	isBlocking		= 0;
	blockCooldown	= 0;
	
	boosterCooldown = 0;
	jumpCooldown	= 0;

	stunCooldown	= 0;
	wasHit			= 0;
	invincibleFrames= 0;

	faceDirection	= 1;
	bodyTobody		= 0;

	hitFloor		= false;
	hitWall			= false;

	health			= 100;
	shield			= 50;

	isExploding		= 0;
	rangeCount		= 0;

	exploded		= 0;

	local_graph.init();

}

Player::~Player(){}

//Update the player
void Player::update(Assets &assets, int playerIDNum,float t, Player &otherPlayer, NodeGraph &world_graph)
{
	//allowing other methods in class to affect the world_graph and other player
	internalOtherPlayer = &otherPlayer;
	internalWorldgraph = &world_graph;
	
	this->updateCooldown();
	this->updateAttack();
	this->updateCollision(assets,playerIDNum,otherPlayer,t);
	
	Animations->update(t);
	playerObject = Animations->getOBJ();
	
	this->updatePos(t);
	this->updateWorldGraph(world_graph);
}

void Player::updateCollision(Assets &assets,int playerIDNum, Player &otherPlayer,float t)
{
	//Check collisions against the other player
	checkOtherPlayer(otherPlayer);

	std::vector<collisionObjects> tempBoundBoxes;
	for(unsigned int i = 0; i < assets.objects.size(); ++i)
	{
		tempBoundBoxes.push_back(assets.objects[i].getHitBox());
	}
	floorPos		= tempBoundBoxes[0].getPos().y;
	leftWallPos		= tempBoundBoxes[1].getPos().x;
	rightWallPos	= tempBoundBoxes[2].getPos().x;
	floorSize		= tempBoundBoxes[0].getSize().y;
	leftWallSize	= tempBoundBoxes[1].getSize().x;
	rightWallSize	= tempBoundBoxes[2].getSize().x;

	bool hitAnything = false;
	int whatHit;

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
}

void Player::updateWorldGraph(NodeGraph &world_graph){
    int temp_xsize = (int)(playerObject.getHitBox().getSize().x/2);
    int temp_ysize = (int)(playerObject.getHitBox().getSize().y/2);

	//clear previous player location on world graph
    for(int i = -temp_xsize; i < temp_xsize; ++i){
        for(int j = -temp_ysize; j < temp_ysize; ++j){
            int temp_row = ((2+i+prevpos.x + world_graph.size)/2);
            int temp_col = ((2+j+prevpos.y + world_graph.size)/2);

            if((temp_row < 0) || (temp_row > world_graph.size-1))
                temp_row = 0;
            if((temp_col < 0) || (temp_col > world_graph.size-1))
                temp_col = 0;

            world_graph.graph[temp_row][temp_col]->visited = false;
			local_graph.graph[temp_row][temp_col]->visited = false;
        }
    }

	//set target for range attack
	int targetxpos;
	if(faceDirection == 1)
		targetxpos = ((position.x+world_graph.size)/2) - (playerObject.getHitBox().getSize().x/2) + 1;
	else
		targetxpos = ((position.x+world_graph.size)/2)  + (playerObject.getHitBox().getSize().x/2) - 1;
	int targetypos = 1+((position.y+world_graph.size)/2);
	targetNode = local_graph.graph[targetxpos][targetypos];

	//set current player position on graph
    for(int i = -temp_xsize; i < temp_xsize; ++i){
        for(int j = -temp_ysize; j < temp_ysize; ++j){
            int temp_row = ((2+i+position.x + world_graph.size)/2);
            int temp_col = ((2+j+position.y + world_graph.size)/2);

            if((temp_row < 0) || (temp_row > world_graph.size-1))
                temp_row = 0;
            if((temp_col < 0) || (temp_col > world_graph.size-1))
                temp_col = 0;

            world_graph.graph[temp_row][temp_col]->visited = true;
			local_graph.graph[temp_row][temp_col]->visited = true;
        }
    }
}

void Player::updateCooldown(){
    if(onCooldown > 0){
        --onCooldown;
    }
    if(isBlocking > 0){
        --isBlocking;
        blockBox.setPos(position);
    } else {
        blockBox.setPos(0,0,500);
    }
    if(blockCooldown > 0){
        --blockCooldown;
    }
    if(boosterCooldown > 0){
        --boosterCooldown;
    }
    if(isDashing > 0){
        --isDashing;
    }
    if(jumpCooldown > 0){
        --jumpCooldown;
    }
    if(stunCooldown > 0){
        --stunCooldown;    
    }
    if(wasHit > 0){
        --wasHit;
    }
    if(invincibleFrames > 0){
        --invincibleFrames;
    }
    if(rangeCooldown > 0){
        --rangeCooldown;
    }
	if(isExploding > 0){
		--isExploding;
	}
}

void Player::updateAttack(){
	//if punching
	if(isAttacking == 6){
		//allow block or dash cancelling
		if((!hitWall) && (isBlocking == 0) && (isDashing == 0)){
			//set hitbox position if dt is under 1 else reset everything
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
			//reset on dash or block cancel
			isAttacking = 0;
			dt = 0.f;
			tInterval = 0.f;
			attackFist.setPos(0,0,500);
		}
	}
	//if kicking
	if(isAttacking == 8){
		//dash or block cancel
		if((!hitWall) && (isBlocking == 0) && (isDashing == 0)){
			//update kick hitbox position
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
				//check to see if they should be rising or diving on the kick
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
	if(isAttacking == 0){
		isKicking = 0;
	}

	//range attack
	if((isAttacking == 11)||(rangeCount == 1))
	{
		//if particle has not traveled full path or hit anything, update position
		if((rdt < 1)&&(isExploding == 0))
		{
			rdt = rangeAttackPath.Update(rtInterval);
			
			attackRange.setPos(rangeAttackPath.GetCurrentState());

			//Create a particle effect for the range attack during it's flight
			ParticleEmitter rangeAttackParticle;

			rangeAttackParticle.setNumOfParticles(30);

			Range lifeRange;
			lifeRange = Range(1,5);
			rangeAttackParticle.setLifeRange(lifeRange);

			Range posRange[3];
			posRange[0] = Range(attackRange.getPos().x-1, attackRange.getPos().x+1);
			posRange[1] = Range(attackRange.getPos().y-1, attackRange.getPos().y+1);
			posRange[2] = Range(attackRange.getPos().z-1, attackRange.getPos().z+1);
			rangeAttackParticle.setPosRange(posRange);

			Range szeRange[3];
			szeRange[0] = Range(1,5);
			szeRange[1] = Range(1,5);
			szeRange[2] = Range(1,5);
			rangeAttackParticle.setSizeRange(szeRange);

			Range aclRange[3];
			aclRange[0] = Range(-1, +1);
			aclRange[1] = Range(-1, +1);
			aclRange[2] = Range(-1, +1);
			rangeAttackParticle.setAcelRange(aclRange);

			Range velRange[3];
			velRange[0] = Range(-1, +1);
			velRange[1] = Range(-1, +1);
			velRange[2] = Range(-1, +1);
			rangeAttackParticle.setVelRange(velRange);

			Range clrRange[3];
			clrRange[0] = Range(250, 255);
			clrRange[1] = Range(250, 255);
			clrRange[2] = Range(250, 255);
			rangeAttackParticle.setColourRange(clrRange);

			rangeAttackParticle.initialise();

			rangeAttackParticle.parent = &particlemanager;
			rangeAttackParticle.particleTexture = rangeAttackParticle.parent->sparkTex;

			particlemanager.addEmmiter(rangeAttackParticle);
		}
		//if particle is exploding, set appropriate cooldowns and set rdt to 1
		else if (isExploding != 0){
			rdt = 1;

			//Create a particle effect for the range attack for when it explodes
			ParticleEmitter rangeAttackParticle;

			rangeAttackParticle.setNumOfParticles(30);

			Range lifeRange;
			lifeRange = Range(1,5);
			rangeAttackParticle.setLifeRange(lifeRange);

			Range posRange[3];
			posRange[0] = Range(attackRange.getPos().x-1, attackRange.getPos().x+1);
			posRange[1] = Range(attackRange.getPos().y-1, attackRange.getPos().y+1);
			posRange[2] = Range(attackRange.getPos().z-1, attackRange.getPos().z+1);
			rangeAttackParticle.setPosRange(posRange);

			Range szeRange[3];
			szeRange[0] = Range(20,30);
			szeRange[1] = Range(20,30);
			szeRange[2] = Range(20,30);
			rangeAttackParticle.setSizeRange(szeRange);

			Range aclRange[3];
			aclRange[0] = Range(-1, +1);
			aclRange[1] = Range(-1, +1);
			aclRange[2] = Range(-1, +1);
			rangeAttackParticle.setAcelRange(aclRange);

			Range velRange[3];
			velRange[0] = Range(-1, +1);
			velRange[1] = Range(-1, +1);
			velRange[2] = Range(-1, +1);
			rangeAttackParticle.setVelRange(velRange);

			Range clrRange[3];
			clrRange[0] = Range(250, 255);
			clrRange[1] = Range(250, 255);
			clrRange[2] = Range(250, 255);
			rangeAttackParticle.setColourRange(clrRange);

			rangeAttackParticle.initialise();

			rangeAttackParticle.parent = &particlemanager;
			rangeAttackParticle.particleTexture = rangeAttackParticle.parent->sparkTex;

			particlemanager.addEmmiter(rangeAttackParticle);
		} 
		else {
			//if it has reached end of path and hasn't exploded, explode
			if(exploded != 1){
				attackRange.setSize(20,20,5);
				if(isExploding == 0)
					isExploding = 10;
				exploded = 1;
			}
			else // reset
			{
				exploded = 0;

				rangeAttackPath.Stop();
				rangeAttackPath.Reset();
				rangeAttackPath.RemoveAllWaypoints();
				internalWorldgraph->reset();
				local_graph.reset();
				attackRange.setSize(1,1,1);
			
				isAttacking = 0;
				rdt   = 0.0f;
				rtInterval = 0.0f;
				attackRange.setPos(0,0,500);
				rangeCount = 0;
			}
		}
	}
}

std::vector<CollisionNode*> Player::rangeAStar(Player &otherPlayer, NodeGraph &local_graph){
	std::vector<CollisionNode*> closedset;//nodes we've already been to
	std::vector<CollisionNode*> openset;//nodes we are going to check
	openset.push_back(targetNode);//start at the beginning, in this case targetNode = behind the player position
	opponentTarget = local_graph.graph[otherPlayer.targetNode->row][otherPlayer.targetNode->column];//the end goal, behind the opponent player
	std::vector<CollisionNode*> failed_path;//return just the start node if this fails
	failed_path.push_back(targetNode);

	targetNode->fromstart = 0;//g score of start node is 0
	targetNode->starttogoal = targetNode->fromstart + distanceBetween(targetNode,opponentTarget);//f score of start node is g score + distance to the end
 
	//run while there are nodes left to check
	while(openset.size() > 0){
		CollisionNode* temp_current = openset[0];
		//find the node with the smallest f score and set temporary current node to it
		for(int i = 0, size = openset.size(); i<size;++i){
			if(openset[i]->starttogoal < temp_current->starttogoal){
				temp_current = openset[i];
			}
		}
		//if we found what we're looking for, return path. Find matches by matching index numbers
		if((temp_current->row == opponentTarget->row)&&(temp_current->column == opponentTarget->column)){
			return reconstructPath(opponentTarget);
		}
		//remove current node from open set since we're checking it
		for(int i = 0;i<openset.size();++i){
			if((temp_current->row == openset[i]->row)&&(temp_current->column == openset[i]->column)){
				openset.erase(openset.begin()+(i));
			}
		}

		//add current node to list of checked nodes
		closedset.push_back(temp_current);

		//check all neighbours
		for(int i = 0, size = temp_current->neighbours.size(); i < size; ++i){
			float temp_fromstart = temp_current->fromstart + distanceBetween(temp_current,temp_current->neighbours[i]);
			float temp_starttogoal = temp_fromstart + distanceBetween(temp_current->neighbours[i], opponentTarget);
			int inClosed = 0;
			int inOpen = 0;
			//check if a neighbour is in the closed set
			for(int j = 0, size = closedset.size(); j<size;++j){
				if((temp_current->neighbours[i]->row == closedset[j]->row)&&(temp_current->neighbours[i]->column==closedset[j]->column)){
					inClosed = 1;
				}
			}
			//check if a neighbour is in the open set
			for(int j = 0, size = openset.size(); j<size;++j){
				if((temp_current->neighbours[i]->row == openset[j]->row)&&(temp_current->neighbours[i]->column==openset[j]->column)){
					inOpen = 1;
				}
			}
			//calculate f_score for all neighbours
		   temp_current->neighbours[i]->starttogoal = distanceBetween(temp_current->neighbours[i],opponentTarget);
		   //if a neighbour is in the closed set, do nothing
		   if((inClosed == 1) &&(temp_starttogoal >= temp_current->neighbours[i]->starttogoal)){
       
			} 
		   //if neighbour is not in the open set or if distance between current to neighbout to goal is smaller than distance between neighbour and goal, set parents and the f and g scores.
		   else if((inOpen != 1)||(temp_starttogoal < temp_current->neighbours[i]->starttogoal)){
			    temp_current->neighbours[i]->camefrom = temp_current;
			    temp_current->neighbours[i]->hasParent = 1;
			    temp_current->neighbours[i]->fromstart = temp_fromstart;
			    temp_current->neighbours[i]->starttogoal = temp_starttogoal;
			    if(inOpen != 1)
					openset.push_back(temp_current->neighbours[i]);
			}
		}
	}
	return failed_path;
}

float Player::distanceBetween(CollisionNode* a, CollisionNode* b){
	//if a node is occupied, can't go there
	if((a->visited)||(b->visited))
		return 100000000000000000;
	else{
		//if node is diagonal then distance is 2, adjacent is 1
		int xdist = abs(a->row-b->row);
		int ydist = abs(a->column-b->column);
		int i = 0;
		while((xdist>0) || (ydist>0)){
			if((xdist>0)&&(ydist>0)){
				xdist -= 1;
				ydist -= 1;
				++i;
				++i;
			} else if(xdist > 0){
			    xdist -= 1;
			    ++i;
			} else if(ydist > 0){
			    ydist -= 1;
			    ++i;
			}
		}
		return i;
	}
}

std::vector<CollisionNode*> Player::reconstructPath( CollisionNode* current_node){
	std::vector<CollisionNode*> shortest_path;

	//recursively backtrack through the node parents and push them into a list ot create a path
	if(current_node->hasParent != 0){
		//std::cout<<current_node->hasParent<<" 1 : "<<current_node->row<<" : "<<current_node->column<<std::endl;
		shortest_path = reconstructPath(current_node->camefrom);
		shortest_path.push_back(current_node);
		return shortest_path;
	} else {
		//std::cout<<current_node->hasParent<<" 2 : "<<current_node->row<<" : "<<current_node->column<<std::endl;
		shortest_path.push_back(current_node);
		return shortest_path;
	}
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
			if((isBlocking>0) && (shield !=0)){
				if(shield > 40)
					shield -= 40;
				else if (shield > 0)
					shield = 0;
			} else {
				if(health > 5)
					health -= 5;
			}
   
			stunCooldown = 50;
			invincibleFrames = 10;
			wasHit = 2;

			otherPlayer.attackRange.setSize(20,20,5);
			if(otherPlayer.isExploding == 0)
			{
				otherPlayer.isExploding = 10;
			}

			glm::vec3 normForce = glm::normalize(glm::vec3(position - otherPlayer.targetNode->position));

			impactForce=glm::vec3(normForce.x*500000,normForce.y*500000,0);
		}
	}
	//if player is colliding with another player
	if(isBoxBoxColliding(playerObject.getHitBox().getPos(),playerObject.getHitBox().getSize(),otherPlayer.getObject().getHitBox().getPos(),otherPlayer.getObject().getHitBox().getSize())){
	}
}

void Player::updatePos(float t){
	prevpos = position;
	//if going t0o fast, no more jumping allowed
	if(velocity.y > maxVelY){
			jumpForce = glm::vec3(0,0,0);
	}
	//change drag force depending on whether or not you're on the ground
	if((!hitFloor)||((moveForce.x < 0)&&(velocity.x < 0))||((moveForce.x>0)&&(velocity.x > 0))){
		resistanceForce.x = -1 * velocity.x;
	} else {
		resistanceForce.x = -200 * velocity.x;
	}
	if((!hitFloor)||((moveForce.z < 0)&&(velocity.z < 0))||((moveForce.z>0)&&(velocity.z > 0))){
	    resistanceForce.z = -1 * velocity.z;
	} else {
	    resistanceForce.z = -200 * velocity.z;
	}

	//if you aren't on the ground, gravity is immense, otherwise meh
	if(!hitFloor)
	{
		gravityForce.y = -5000.0f;
	} else {
		gravityForce.y = 0.f;
	}
	//at top of jump, make gravity stronger to avoid floating
	if((velocity.y > -1.f) && (velocity.y < 1.f)&&(jumpCount != 0)){
		gravityForce = glm::vec3(0,-5000,0);
	} else if (jumpCount != 0) {
		gravityForce += glm::vec3(0,-100,0);
	}
	
	//if player isn't recieving move input, kill move force
	if(isMoving == 0){
	    moveForce = glm::vec3(0,0,0);
	}
	//damp all forces if currently blocking
	if(isBlocking > 0){
	    jumpForce = jumpForce * 0.01f;
	    moveForce = moveForce*0.01f;
	    impactForce = impactForce*0.1f;
	}
	//if you aren't hit set impact to 0
	if(wasHit == 0){
		impactForce = glm::vec3(0,0,0);
	}
	//not using this but it's supposed to be for player -> player collisions
	if(bodyTobody == 0){
		pushForce = glm::vec3(0,0,0);
	}
	//total force
	totalForce = impactForce + gravityForce + moveForce + resistanceForce + jumpForce + pushForce;
	//z axis position
	acceleration.z = t*totalForce.z;
	velocity.z += t*acceleration.z;
	position.z += t*velocity.z + 0.5f*acceleration.z*t*t;
	
	//if you aren't on ground, update position accordingly
	if(!hitFloor)
	{
		jumpCount = 1;
		acceleration.y = t*totalForce.y;
		velocity.y += t*acceleration.y;
		position.y += t*velocity.y + 0.5f*acceleration.y*t*t;
	}
	else if(hitFloor)//if on the ground
	{
		
		//set position, using force for this creates skittering on the ground
		position.y = floorPos + (floorSize/2) + (playerObject.getHitBox().getSize().y/2);

		gravityForce.y	= 0.0f;
		acceleration.y	= 0.0f;
		
		//disgusting, I know, but this is where player velocity changes if they are kicking
		if(isKicking == 1)
			isKicking = 0;
		if(isKicking == 0)
			velocity.y	= 0.0f;
		else if (isKicking > 1)
			velocity.y = 75.f;

		//to prevent player from holding down jump and having them rocket into space
		if(jumpCount > 0){
			acceleration.y = t*totalForce.y;
			velocity.y += t*acceleration.y;
			position.y += t*velocity.y + 0.5f*acceleration.y*t*t;
		}
		//reset jump count
		jumpCount		= 0;
	}

	//wall collision
	if(!hitWall)
	{
		acceleration.x = t*totalForce.x;
		velocity.x += t*acceleration.x;
		position.x += t*velocity.x + 0.5f*acceleration.x*t*t;
	}
	else if(hitWall)
	{
		position.x = position.x - t*velocity.x - 0.5f*acceleration.x*t*t;

		totalForce.x	= impactForce.x = gravityForce.x = moveForce.x = resistanceForce.x = jumpForce.x = 0.0f;
		acceleration.x	= 0.0f;
		velocity.x		= 0.0f;
	}

	//Update the rotation based on what direction the player is facing
	if (faceDirection == 1)
		rotation = glm::vec3(0, 90, 0);
	else
		rotation = glm::vec3(0, -90, 0);

	//Update object and hitbox
	playerObject.setPos(position);
	playerObject.setRotation(rotation);
	playerObject.getHitBox().setPos(position.x, position.y, position.z);
}

//draw player
void Player::draw()
{
	////Bind the texture
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, this->getObject().getTex());          //TODO: Not sure if this is needed for shader texturing, will leave for now 

	//Render VBO
	playerObject.drawOBJ();

	particlemanager.update(1.f);
	particlemanager.draw();
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
				 break;
			case 9:
			    moveForce = glm::vec3(0,0,2000);
			    isMoving = numAction;
			    break;
			case 10:
			    moveForce = glm::vec3(0,0,-2000);
			    isMoving = numAction;
			    break;
			case 11:
			    if(rangeCooldown == 0){
			        this->attackAction(numAction);
			        isAttacking = numAction;
			        isMoving = 0;
			    }
			    break;
			default:
				isMoving = 0;
				break;
				//idle
		}
	}
}

//movement + any collisions
void Player::moveAction(int numAction){
	//set movement forces
	if(numAction == 1){
		 moveForce = glm::vec3(2000,0,0);
	} else if (numAction == 2){
	    moveForce = glm::vec3(-2000,0,0);
	} else if (numAction == 3){// set boost forces and particles
	    boosterCooldown = 60;
		isDashing = 2;

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
        
        tempRange[0] = Range(0,20);
        tempRange[1] = Range(0,20);
        tempRange[2] = Range(0,1);
        newemitter.setSizeRange(tempRange);

        tempRange[0] = Range(250,256);
        tempRange[1] = Range(250,256);
        tempRange[2] = Range(250,256);
        newemitter.setColourRange(tempRange);

        newemitter.initialise();

		newemitter.parent = &particlemanager;
		newemitter.particleTexture = newemitter.parent->smokeTex;

        particlemanager.addEmmiter(newemitter);
		moveForce = glm::vec3(150000,0,0);
	} else if (numAction == 4){
		boosterCooldown = 60;
		isDashing = 2;
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
		
		tempRange[0] = Range(0,20);
		tempRange[1] = Range(0,20);
		tempRange[2] = Range(0,1);
		newemitter.setSizeRange(tempRange);
		
		tempRange[0] = Range(250,256);
		tempRange[1] = Range(250,256);
		tempRange[2] = Range(250,256);
		newemitter.setColourRange(tempRange);
		
		newemitter.parent = &particlemanager;
		newemitter.particleTexture = newemitter.parent->smokeTex;

		newemitter.initialise();
		particlemanager.addEmmiter(newemitter);
		moveForce = glm::vec3(-150000,0,0);
	} else if (numAction == 5){//set jump force and particles
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
			
			tempRange[0] = Range(0,20);
			tempRange[1] = Range(0,20);
			tempRange[2] = Range(0,1);
			newemitter.setSizeRange(tempRange);
			
			tempRange[0] = Range(250,256);
			tempRange[1] = Range(250,256);
			tempRange[2] = Range(250,256);
			newemitter.setColourRange(tempRange);

			newemitter.parent = &particlemanager;
			newemitter.particleTexture = newemitter.parent->smokeTex;

			newemitter.initialise();
			particlemanager.addEmmiter(newemitter);
			jumpForce = glm::vec3(0,120000,0);
			++jumpCount;
		}
	}
}

void Player::attackAction(int numAction){
    isAttacking = numAction;
    if(numAction == 6){//punch
		Animations->setAnimation(1);
        tInterval = 0.15f;
        attackFist.setPos(position.x+8,position.y+3.5,position.z);
        attackStartPos = attackFist.getPos();
        onCooldown = 20;
    } else if(numAction == 8){//kick
		if(hitFloor)
			Animations->setAnimation(2);//If on floor, ground kick
		else
			Animations->setAnimation(3);//If in air, air kick
        tInterval = 0.05f;
        attackKick.setPos(position.x+8,position.y-3.5,position.z);
        attackStartPos = attackKick.getPos();
        onCooldown = 40;
		isKicking = 1;

    } else if (numAction == 11){//range

		if(rangeCount == 0)
		{
			rangeCount = 1;
			//Generate path from A* to be used for the tragectory of the ranged attack
			std::vector<CollisionNode *> V;	
			V =  rangeAStar(*internalOtherPlayer,local_graph);
			for(unsigned int i = 0; i < V.size(); ++i)
			{
				rangeAttackPath.AddWaypointToEnd(V[i]->position);
			}
			V.clear();
			rangeAttackPath.genCatmullTable();
			rangeAttackPath.Start();

			rtInterval = 0.02f;
			attackRange.setPos(position.x+5,position.y+5,position.z);
			attackStartPos = attackRange.getPos();
			rangeCooldown = 60;
		}
	}
}

void Player::blockAction(int numAction){
    isBlocking = 20;
    blockCooldown = 30;
	blockBox.setPos(position);
	Animations->setAnimation(4);

}
//set particles on death
void Player::Death()
{
	ParticleEmitter deathEmitter;

	deathEmitter.setNumOfParticles(200);

	Range lifeRange;
	lifeRange = Range(0,3);
	deathEmitter.setLifeRange(lifeRange);

	Range szeRange[3];
	szeRange[0] = Range(0, 4);
	szeRange[1] = Range(0, 4);
	szeRange[2] = Range(0, 4);
	deathEmitter.setSizeRange(szeRange);

	Range posRange[3];
	posRange[0] = Range(position.x,position.x+1);
	posRange[1] = Range(position.y,position.y+1);
	posRange[2] = Range(position.z,position.z+1);
	deathEmitter.setPosRange(posRange);

	Range clrRange[3];
	clrRange[0] = Range(200,250);
	clrRange[1] = Range(200,250);
	clrRange[2] = Range(200,250);
	deathEmitter.setColourRange(clrRange);

	Range velRange[3];
	velRange[0] = Range(0,1);
	velRange[1] = Range(0,1);
	velRange[2] = Range(0,1);
	deathEmitter.setAcelRange(velRange);

	Range aclRange[3];
	aclRange[0] = Range(0,1);
	aclRange[1] = Range(0,1);
	aclRange[2] = Range(0,1);
	deathEmitter.setAcelRange(aclRange);

	deathEmitter.initialise();

	for(unsigned int i = 0; i < deathEmitter.particleList.size(); ++i)
	{
		deathEmitter.particleList[i].position/10.0f;
		deathEmitter.particleList[i].velocity/10.0f;
		deathEmitter.particleList[i].acceleration/10.0f;
	}

	deathEmitter.parent = &particlemanager;

	deathEmitter.particleTexture = deathEmitter.parent->sparkTex;

	particlemanager.addEmmiter(deathEmitter);
}

//getters and setters, good chunk of these are unused

//object
OBJModel Player::getObject(){
	return playerObject;
}
void setObject(OBJModel object);

//position
glm::vec3 Player::getPos(){return glm::vec3(position.x,position.y, position.z);}
void Player::setPos(glm::vec3 newposition){	position = newposition; }

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

//Animations
PlayerAnimation::PlayerAnimation(OBJModel &object, Assets &targets)
{
	currentAnimation = 0;
	currentFrame = 0;
	nextFrame = 1;
	playerObject = object;
	morphTargets = targets;
	dt = 0.0f;
}

//update animation
void PlayerAnimation::update(float t)
{
	dt += t;

	itsMorphingTime();

	if(dt >= 1.0f){
		currentFrame = nextFrame;
		nextFrame++;
		dt = 0.0f;
	}

	if(nextFrame == animations[currentAnimation].size())
	{
		setAnimation(0);
	}
		
}


/*
//Morph targets are going to be replaced with skeleten animation and this is currently blocking from complining
//TODO: Replace with skeleten animation
*/

//morph target
void PlayerAnimation::itsMorphingTime()
{
	//int size = morphTargets.objects[currentFrame].getVerSize();  //TODO Implementing size check of objects interpolating to
	//for(int i=0; i<size ;i++){
	//	int p0 = animations[currentAnimation][currentFrame];
	//	int p1 = animations[currentAnimation][nextFrame];
	//	playerObject.setVertex(LERP(morphTargets.objects[p0].getVertex(i),morphTargets.objects[p1].getVertex(i),dt),i);
	//}	
}