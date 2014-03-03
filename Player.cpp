#include "Player.h"


Player::Player(){
	pos = glm::vec3(0,0,0);
	vel = glm::vec3(0,0,0);

	hp = 1000;
	sp = 1000;

	//default face right, set in update
	isFacing = 1;
	onGround = 1;

	for(int i = 0; i < 5; ++i){
		Hitbox temp;
		hitboxList.push_back(temp);
	}
	
	//init hitboxes
	hitboxList[BODYBOX ].isActive = 1;
	hitboxList[PUNCHBOX].isActive = 0;
	hitboxList[KICKBOX ].isActive = 0;
	hitboxList[LASERBOX].isActive = 0;
	hitboxList[BLASTBOX].isActive = 0;

	hitboxList[BODYBOX ].pos = pos;
	hitboxList[PUNCHBOX].pos = glm::vec3(pos.x+(5*isFacing),pos.y+10,1000);
	hitboxList[KICKBOX ].pos = glm::vec3(pos.x+(5*isFacing), pos.y-10,1000);
	hitboxList[LASERBOX].pos = glm::vec3(pos.x+(20*isFacing), pos.y, 1000);
	hitboxList[BLASTBOX].pos = glm::vec3(pos.x,pos.y,1000);
		
	//action list is IDLE
	prevAction    = IDLE;
	currentAction = IDLE;
	nextAction    = IDLE;

	//TODO calculate this, used for time passed
	freq = 1/60;

	actionTimer = 0;

	//sp regen freeze
	blastStunCooldown = 0;
}
Player::~Player(){}
void Player::update(int collision, int action, Player otherPlayer){
	
	//if player is not in the middle of an action and 
	if(currentAction == IDLE && otherPlayer.pos.x > pos.x){
		isFacing = 1;
	} else {
		isFacing = -1;
	}

	//check controller input, update current action if possible
	//actions return idle when completed

	//TODO following line needs to be replaced with actual input
	nextAction = controllerInput(IDLE);

	//following determines which move is executed next
	//cancelling moves, no else statements so the lower if statements get priority
	//dash cancel
	if(nextAction == DASH_LEFT || nextAction == DASH_RIGHT){
		//can only dash cancel out of move
		if(currentAction == MOVE_LEFT || currentAction == MOVE_RIGHT){
			cycleActions();
		}
	}

	//jumping
	if(nextAction == JUMP){
		//jump cancel out of move and dash
		if(currentAction == MOVE_LEFT || currentAction == MOVE_RIGHT
			|| currentAction == DASH_LEFT || currentAction == DASH_RIGHT){
			cycleActions();
		}
	}

	//attacks
	if(nextAction == PUNCH || nextAction == KICK 
		|| nextAction == LASER || nextAction == BLAST){
		//cancel out of move, dash, jump
		if(currentAction == MOVE_LEFT || currentAction == MOVE_RIGHT
			|| currentAction == DASH_LEFT || currentAction == DASH_RIGHT
			|| currentAction == JUMP){
			cycleActions();
		}
	}

	//block
	if(nextAction == BLOCK && sp > 10){
		//cancel out of move,dash,jump,attacks
		if(currentAction == MOVE_LEFT || currentAction == MOVE_RIGHT
			|| currentAction == DASH_LEFT || currentAction == DASH_RIGHT
			|| currentAction == JUMP
			|| currentAction == PUNCH || currentAction == KICK || currentAction == LASER || currentAction == BLAST){
			cycleActions();
		}
	}

	//INTERRUPTS GO BEFORE ACTUAL UPDATING
	//check collisions
	for(int i = 0, size = otherPlayer.hitboxList.size(); i < size; ++ i){
		//check collisions with self body
		//TODO implement ground collision detection
		if(onGround){
			onGround = 1;
		} else {
			onGround = 0;
		}
		//TODO replace boxes with actual collision detection
		if(BODYBOX){
			//do nothing, maybe greatly reduce velocity?
		}else if (PUNCHBOX || KICKBOX || LASERBOX || BLASTBOX && currentAction != BLOCK){
			//if hit by attack, interrupt current and trigger stagger if not blocking
			actionTimer = 0;
			prevAction = currentAction;
			nextAction = IDLE;
			if(onGround){
				currentAction = playerAction.staggerAAction(actionTimer,vel,isFacing, hitboxList);
			} else {
				currentAction = playerAction.staggerGAction(actionTimer,vel,isFacing, hitboxList);
			}

			//since not blocking, reduce hp/sp according to attack TODO collision shiiit
			if(PUNCHBOX){
				hp -= 150;
			} else if (KICKBOX){
				hp -= 200;
			} else if (LASERBOX){
				hp -= 100;
			} else if (BLASTBOX){
				hp -= 50;
				sp -= 500;
				blastStunCooldown = 60;
			}
		} else if (PUNCHBOX || KICKBOX || LASERBOX || BLASTBOX && currentAction == BLOCK){
			if(PUNCHBOX){
				hp -= 20;
				sp -= 100;
			} else if (KICKBOX){
				hp -= 150;
				sp -= 50;
			} else if (LASERBOX){
				sp -= 300;
			} else if (BLASTBOX){
				sp -= 200;
				blastStunCooldown = 60;
			}
		}
	}

	//ACTUAL UPDATING OF VEL BELOW
	
	//if action is complete, returns IDLE
	if(currentAction == MOVE_LEFT){
		currentAction = playerAction.moveLeftAction(actionTimer,vel);
	} else if(currentAction == MOVE_RIGHT){
		currentAction = playerAction.moveRightAction(actionTimer,vel);
	} else if(currentAction == DASH_LEFT){
		currentAction = playerAction.dashLeftAction(vel);
	} else if(currentAction == DASH_RIGHT){
		currentAction = playerAction.dashRightAction(vel);
	} else if(currentAction == JUMP){
		currentAction = playerAction.jumpAction(vel);
	} else if(currentAction == PUNCH){
		currentAction = playerAction.punchAction(actionTimer,vel,isFacing, hitboxList);
	} else if(currentAction == KICK){
		currentAction = playerAction.kickAction(actionTimer,vel,isFacing, onGround, hitboxList);
	} else if(currentAction == LASER){
		currentAction = playerAction.laserAction(actionTimer,vel,isFacing, hitboxList);
	} else if(currentAction == BLAST){
		currentAction = playerAction.blastAction(actionTimer,vel, hitboxList);
	} else if(currentAction == BLOCK){
		currentAction = playerAction.blockAction(actionTimer,vel, hitboxList, sp);
	} else if(currentAction == STAGGER_G){
		currentAction = playerAction.staggerGAction(actionTimer,vel,isFacing,hitboxList);
	} else if(currentAction == STAGGER_A){
		currentAction = playerAction.staggerAAction(actionTimer,vel,isFacing,hitboxList);
	}

	//cycle actions if IDLE
	if(currentAction == IDLE){
		cycleActions();
	}

	//x velocity damping
	if(vel.x > 2){
		if(onGround == 1)
			vel.x -= 3;
	} else if(vel.x < -2){
		if(onGround == 1)
			vel.x += 3;
	}
	if(vel.x < 2 && vel.x > -2)
		vel.x = 0;

	//gravity
	vel.y -=3;

	//actual position update
	pos.x = vel.x * freq;
	pos.y = vel.y * freq;

	//cheating on ground collisions
	if(pos.y < 0){
		pos.y = 0;
		//cancels gravity
		vel.y += 3;
	}
	
	//cheating on walls
	if(pos.x > 400)
		pos.x = 400;
	if(pos.x < -400)
		pos.x = -400;

	//regen SP unless frozen by blast attack
	if(blastStunCooldown != 0){
		sp += 10;
		blastStunCooldown -= 1;
	}
	if(sp > 1000)
		sp = 1000;

	//lower limits
	if(hp < 0)
		hp = 0;
	if(sp < 0)
		sp = 0;

}
void draw(){

}

Actions Player::controllerInput(Actions action){

	//action cooldown checks in here so jump and dash spamming aren't possible
	
	
	return action;
}


//moves actions to next actions
void Player::cycleActions(){
	prevAction = currentAction;
	currentAction = nextAction;
	nextAction = IDLE;
	actionTimer = 0;
}