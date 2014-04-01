#include "Player.h"


Player::Player(){
	sceneGraphNode = this->GetNode();

	vel = glm::vec3(0,0,0);

	hp = maxHP = 1000;
	sp = maxSP = 1000;

	//default face right, set in update
	isFacing = 1;
	onGround = 1;

	for(int i = 0; i < 5; ++i){
		CollisionBox temp;
		this->GetNode()->AttachNode(temp.GetSceneGraphObject());
		hitboxList.push_back(temp);
	}
	
	//init hitboxes
	hitboxList[BODYBOX].GetCollisionBox()->SetIsActive(true);
	hitboxList[PUNCHBOX].GetCollisionBox()->SetIsActive(false);
	hitboxList[KICKBOX].GetCollisionBox()->SetIsActive(false);
	hitboxList[LASERBOX].GetCollisionBox()->SetIsActive(false);
	hitboxList[BLASTBOX].GetCollisionBox()->SetIsActive(false);

	hitboxList[BODYBOX].GetCollisionBox()->GetSceneGraphObject()->TranslateNode(glm::vec3(0.0f, 0.0f, 0.0f));						//.pos		= pos;
	hitboxList[BODYBOX].GetCollisionBox()->GetSceneGraphObject()->SetScale(glm::vec3(5.0f, 15.0f, 8.0f));

	hitboxList[PUNCHBOX].GetCollisionBox()->GetSceneGraphObject()->SetLocalPosition(glm::vec3(0,0,1000));
	hitboxList[KICKBOX].GetCollisionBox()->GetSceneGraphObject()->SetLocalPosition(glm::vec3(0,0,1000));
	hitboxList[LASERBOX].GetCollisionBox()->GetSceneGraphObject()->SetLocalPosition(glm::vec3(0,0,1000));
	hitboxList[BLASTBOX].GetCollisionBox()->GetSceneGraphObject()->SetLocalPosition(glm::vec3(0,0,1000));
	hitboxList[LASERBOX].GetCollisionBox()->GetSceneGraphObject()->TranslateNode(glm::vec3(0.0f, 0.0f, 1000.0f));
	hitboxList[LASERBOX].GetCollisionBox()->GetSceneGraphObject()->SetScale(glm::vec3(40.f, 3.f, 2.0f));
		
	//action list is IDLE
	prevAction    = IDLE;
	currentAction = IDLE;
	nextAction    = IDLE;

	//TODO calculate this, used for time passed
	freq = 1.f/60.f;

	actionTimer = 0;

	//sp regen freeze
	blastStunCooldown = 0;

	//TODO KILL
	tempTimer = 0;

	maxDistBetweenPlayers = 60.f;

	hasBeenHit = 0;

}
Player::~Player(){}
void Player::update(Player *otherPlayer, playerSFX &sfx){

	pos = this->GetNode()->GetWorldPosition();

	//if player is not in the middle of an action and 
	if (currentAction == IDLE && otherPlayer->pos.x > pos.x){
		isFacing = 1;
	}
	else if (currentAction == IDLE && otherPlayer->pos.x < pos.x) {
		isFacing = -1;
	}

	//prevents doubling up
	if (currentAction == nextAction && currentAction != IDLE && currentAction != BLOCK){
		nextAction = IDLE;
	}

	//following determines which move is executed next
	//cancelling moves, no else statements so the lower if statements get priority
	//dash cancel
	if (nextAction == DASH_LEFT || nextAction == DASH_RIGHT){
		//can only dash cancel out of move
		if (currentAction == MOVE_LEFT || currentAction == MOVE_RIGHT){
			cycleActions();
		}
	}

	//jumping
	if (nextAction == JUMP){
		//jump cancel out of move and dash
		if (currentAction == MOVE_LEFT || currentAction == MOVE_RIGHT
			|| currentAction == DASH_LEFT || currentAction == DASH_RIGHT){
			cycleActions();
		}
	}

	//attacks
	if (nextAction == PUNCH || nextAction == KICK
		|| nextAction == LASER || nextAction == BLAST){
		//cancel out of move, dash, jump
		if (currentAction == MOVE_LEFT || currentAction == MOVE_RIGHT
			|| currentAction == DASH_LEFT || currentAction == DASH_RIGHT
			|| currentAction == JUMP){
			cycleActions();
		}
	}

	//block
	if (nextAction == BLOCK && sp > 10){
		//cancel out of move,dash,jump,attacks
		if (currentAction == MOVE_LEFT || currentAction == MOVE_RIGHT
			|| currentAction == DASH_LEFT || currentAction == DASH_RIGHT
			|| currentAction == JUMP
			|| currentAction == PUNCH || currentAction == KICK || currentAction == LASER || currentAction == BLAST){
			cycleActions();
		}
	}

	if(nextAction == STAGGER_A || nextAction == STAGGER_G){
		cycleActions();
	}

	//INTERRUPTS GO BEFORE ACTUAL UPDATING
	//check collisions
	bool hitboxListHit[5];
	for (unsigned int i = 0, size = otherPlayer->hitboxList.size(); i < size; ++i)
	{
		hitboxListHit[i] = this->hitboxList[BODYBOX].CheckCollision(&otherPlayer->hitboxList[i]);
	}
	//check collisions with self body

	//TODO replace boxes with actual collision detection
	if (hitboxListHit[BODYBOX]){
		//do nothing, maybe greatly reduce velocity?
	}
	
	if ((hitboxListHit[PUNCHBOX] || hitboxListHit[KICKBOX] || hitboxListHit[LASERBOX] 
	|| hitboxListHit[BLASTBOX]) && currentAction != BLOCK && hasBeenHit == 0){
		if(!hitboxListHit[LASERBOX]){
			//if hit by attack, interrupt current and trigger stagger if not blocking
			actionTimer = 0;
			prevAction = currentAction;
			nextAction = IDLE;
			if (onGround == 0){
				currentAction = STAGGER_A;
			}
			else {
				currentAction = STAGGER_G;
			}
		}

		//since not blocking, reduce hp/sp according to attack TODO collision shiiit
		if (hitboxListHit[PUNCHBOX]){
			hp -= 150;
		}
		else if (hitboxListHit[KICKBOX]){
			hp -= 200;
		}
		else if (hitboxListHit[LASERBOX]){
			hp -= 100;
		}
		else if (hitboxListHit[BLASTBOX]){
			hp -= 50;
			sp -= 500;
			blastStunCooldown = 60;
		}
		hasBeenHit += 1;
	}
	else if ((hitboxListHit[PUNCHBOX] || hitboxListHit[KICKBOX] || hitboxListHit[LASERBOX] || hitboxListHit[BLASTBOX]) && currentAction == BLOCK && hasBeenHit == 0){
		sfx = BLOCK_IMPACT_SFX;
		soundCheck(sfx);

		if (hitboxListHit[PUNCHBOX]){
			hp -= 20;
			sp -= 100;

			if(otherPlayer->getOnGround() == 0){
				otherPlayer->setNextAction(STAGGER_A);
			} else if(otherPlayer->getOnGround() == 1){
				otherPlayer->setNextAction(STAGGER_G);
			}
		}
		else if (hitboxListHit[KICKBOX]){
			hp -= 150;
			sp -= 50;


			if(otherPlayer->getOnGround() == 0){
				otherPlayer->setNextAction(STAGGER_A);
			} else if(otherPlayer->getOnGround() == 1){
				otherPlayer->setNextAction(STAGGER_G);
			}
		}
		else if (hitboxListHit[LASERBOX]){
			sp -= 300;
		}
		else if (hitboxListHit[BLASTBOX]){
			sp -= 200;
			blastStunCooldown = 60;
		}
		hasBeenHit += 1;
	}
	
	if (sfx != BLOCK_IMPACT_SFX)
	{
		sfx = EMPTY_P_SFX;
	}

	//ACTUAL UPDATING OF VEL BELOW
	//if action is complete, returns IDLE
	if (currentAction == MOVE_LEFT){
		//sfx = MOVE_SFX;
		//soundCheck(sfx);
		
		currentAction = playerAction.moveLeftAction(actionTimer, vel);
		
		this->GetMorphTargets()->SetAnimation(PLAYER_ANIMATION_MOVE);
	}
	else if (currentAction == MOVE_RIGHT){
		//sfx = MOVE_SFX;
		//soundCheck(sfx);
		currentAction = playerAction.moveRightAction(actionTimer, vel);
		
		this->GetMorphTargets()->SetAnimation(PLAYER_ANIMATION_MOVE);
	}
	else if (currentAction == DASH_LEFT){
		//sfx = DASH_SFX;
		//soundCheck(sfx);
		currentAction = playerAction.dashLeftAction(actionTimer, vel, sp);
		
	}
	else if (currentAction == DASH_RIGHT){
		//sfx = DASH_SFX;
		//soundCheck(sfx);
		currentAction = playerAction.dashRightAction(actionTimer, vel, sp);
		
	}
	else if (currentAction == JUMP){
		//sfx = JUMP_SFX;
		//soundCheck(sfx);
		currentAction = playerAction.jumpAction(vel, onGround);
		
	}
	else if (currentAction == PUNCH){
		currentAction = playerAction.punchAction(actionTimer, vel, isFacing, hitboxList, onGround);
		sfx = PUNCH_SFX;
		soundCheck(sfx);

		this->GetMorphTargets()->SetAnimation(PLAYER_ANIMATION_PUNCH);
	}
	else if (currentAction == KICK){
		sfx = KICK_SFX;
		soundCheck(sfx);
		currentAction = playerAction.kickAction(actionTimer, vel, isFacing, onGround, hitboxList);
		
	}
	else if (currentAction == LASER){
		sfx = LASER_SFX;
		soundCheck(sfx);
		currentAction = playerAction.laserAction(actionTimer, vel, isFacing, hitboxList);
		
	}
	else if (currentAction == BLAST){
		sfx = BLAST_SFX;
		soundCheck(sfx);
		currentAction = playerAction.blastAction(actionTimer, vel, hitboxList);
		
	}
	else if (currentAction == BLOCK){
		if(sfx != BLOCK_IMPACT_SFX){
			sfx = BLOCK_IMPACT_SFX;
			actionTimer = 0;
		}
		soundCheck(sfx);
		currentAction = playerAction.blockAction(actionTimer, vel, hitboxList, sp);
		
		this->GetMorphTargets()->SetAnimation(PLAYER_ANIMATION_BLOCK);
	}
	else if (currentAction == STAGGER_G){
		sfx = IMPACT1_SFX;
		soundCheck(sfx);
		currentAction = playerAction.staggerGAction(actionTimer, vel, isFacing, hitboxList, hasBeenHit);
		this->GetMorphTargets()->SetAnimation(PLAYER_ANIMATION_STAGGER);
	}
	else if (currentAction == STAGGER_A){
		sfx = IMPACT2_SFX;
		soundCheck(sfx);
		currentAction = playerAction.staggerAAction(actionTimer, vel, isFacing, hitboxList, hasBeenHit);
		this->GetMorphTargets()->SetAnimation(PLAYER_ANIMATION_STAGGER);
	}
	else
	{
		
	}

	//cycle actions if IDLE
	if (currentAction == IDLE && prevAction == IDLE){
		cycleActions();
		this->GetMorphTargets()->SetAnimation(PLAYER_ANIMATION_IDLE);
	}
	else if ((currentAction == IDLE)||(currentAction == MOVE_LEFT || currentAction == MOVE_RIGHT)){
		cycleActions();
	} 



	//x velocity damping
	if(prevAction!=MOVE_LEFT && prevAction!=MOVE_RIGHT){
		
		if (vel.x > 0){
			if (onGround == 1){
				vel.x -= 15.f;
			} else {
				vel.x -= 5.f;
			}
			if (vel.x < 0)
				vel.x = 0;
		}
		else if (vel.x < 0){
			if (onGround == 1){
				vel.x += 15.f;
			} else {
				vel.x += 5.f;
			}
			if (vel.x > 0)
				vel.x = 0;
		}
	}

	//gravity
	vel.y -= 3;

	//actual position update
	pos.x += vel.x * freq;
	pos.y += vel.y * freq;

	if(abs(pos.x-otherPlayer->pos.x) > maxDistBetweenPlayers){
		pos.x -= vel.x*freq;
	}

	//cheating on ground collisions
	if (pos.y < -5){
		onGround = 1;
		pos.y = -5;
		//cancels gravity
		vel.y = 0;
	}
	else {
		onGround = 0;
	}

	//cheating on walls
	if (pos.x > 400)
		pos.x = 400;
	if (pos.x < -265)
		pos.x = -265;

	//regen SP unless frozen by blast attack
	if (blastStunCooldown != 0){
		blastStunCooldown -= 1;
	}
	else {
		sp += 10;
	}
	if (sp > 1000)
		sp = 1000;

	//lower limits
	if (hp < 0)
		hp = 0;
	if (sp < 0)
		sp = 0;

	this->GetNode()->SetRotation(glm::vec3(isFacing*90, 0, 0));

	//Final update to the scene graph
	this->GetNode()->SetLocalPosition(pos);
}
void draw(){

}

Actions Player::controllerInput(Actions action){

	//action cooldown checks in here so jump and dash spamming aren't possible
	//check controller input, update current action if possible
	//actions return idle when completed
	if(nextAction != STAGGER_A && nextAction != STAGGER_G)
		nextAction = action;

	return action;
}


//moves actions to next actions
void Player::cycleActions(){
	prevAction = currentAction;
	currentAction = nextAction;
	nextAction = IDLE;
	actionTimer = 0;
	hasBeenHit = 0;
	
}

std::vector<CollisionBox> Player::GetCollisionBoxes(void)
{
	return hitboxList;
}

glm::vec3 Player::getPos(){
	return pos;
}

void Player::soundCheck(playerSFX &sfx){
	if(actionTimer != 1){
		sfx = EMPTY_P_SFX;
	}

	
}


void Player::setNextAction(Actions newAction){

nextAction = newAction;

}

int Player::getOnGround(){
 return onGround;
}