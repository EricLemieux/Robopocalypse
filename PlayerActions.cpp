#include "PlayerActions.h"

PlayerActions::PlayerActions(){}
PlayerActions::~PlayerActions(){}


void PlayerActions::activateHitbox(Hitbox &box){
	box.isActive = 1;
	box.pos.z = 0;
}
void PlayerActions::deactivateHitbox(Hitbox &box){
	box.isActive = 0;
	box.pos.z = 1000;
}

//doesn't deactivate body hitbox
void PlayerActions::deactivateAllHitbox(std::vector<Hitbox> &hitboxList){
	deactivateHitbox(hitboxList[PUNCHBOX]);
	deactivateHitbox(hitboxList[KICKBOX]);
	deactivateHitbox(hitboxList[LASERBOX]);
	deactivateHitbox(hitboxList[BLASTBOX]);
}

Actions PlayerActions::moveLeftAction(int &t, glm::vec3 &vel){
	
	int casttime = 120;
	int moveVel = -50;

	if(t>casttime)
		t = casttime;

	//if speed is less than max, increase it otherwise reduce it
	if(vel.x < moveVel)
		vel.x += 5;
	else
		vel.x -= 5;

	t+=1;
	
	return MOVE_LEFT;
}

Actions PlayerActions::moveRightAction(int &t, glm::vec3 &vel){
	
	int casttime = 120;
	int complete = 0;

	int moveVel = 50;

	if(t>casttime)
		t = casttime;

	if(vel.x > moveVel)
		vel.x -= 5;
	else
		vel.x += 5;

	t+=1;
	
	return MOVE_RIGHT;
}

Actions PlayerActions::dashLeftAction(glm::vec3 &vel){

	int dashVel = -100;

	vel.x = dashVel;
	return IDLE;
}

Actions PlayerActions::dashRightAction(glm::vec3 &vel){

	int dashVel = 100;

	vel.x = dashVel;
	return IDLE;
}

Actions PlayerActions::jumpAction(glm::vec3 &vel){
	int cooldown = 30;
	int jumpVel = 50;

	vel.y = jumpVel;

	return IDLE;
}

Actions PlayerActions::punchAction(int &t, glm::vec3 &vel, int facing, std::vector<Hitbox> &hitboxList){
	int vel_curve[4] = {0,1,0.8,0};
	
	int casttime = 20;
	int recovery = 40;

	int punchVel = 10;

	deactivateAllHitbox(hitboxList);
	if(t > (casttime+recovery))
		return IDLE;
	

	if(t<casttime){
		activateHitbox(hitboxList[PUNCHBOX]);
		vel.x = Bezier(vel_curve,1,t/casttime)*punchVel*facing;
		vel.y = 0;
	}

	if(t>casttime)
		deactivateHitbox(hitboxList[PUNCHBOX]);

	t+=1;
	return PUNCH;
}

int kickStart;//0 started on ground, 1 started in air

Actions PlayerActions::kickAction(int &t, glm::vec3 &vel, int facing, int onGround, std::vector<Hitbox> &hitboxList){
	int vel_curveup[4] = {0,0.5,0.5,0};
	int vel_curvedown[4] = {0.5,0.75,0.75,1};
	glm::vec3 kickVel;
	int casttime = 60;
	int recovery = 60;

	deactivateAllHitbox(hitboxList);

	if(t>casttime+recovery)
		return IDLE;
	
	//initial check for starting point, if 0 starting from ground
	if((t == 0) && (onGround == 1)){
		kickStart = 0;
	} else if ((t == 0) && (onGround == 0)){
		kickStart = 1;
	}


	if(t < casttime){
		activateHitbox(hitboxList[KICKBOX]);
		//if starting from the ground, kick up at an angle
		if(kickStart == 0){
			kickVel = glm::vec3(30,10,0);
			kickVel.y = Bezier(vel_curvedown,1,t/casttime)*kickVel.y*facing;
		} else if (kickStart == 1){ // diiiiive kiiiickuuuu
			//if hit the ground, stop
			if(onGround == 1){
				t = casttime+1;
				vel.x = 0;
				vel.y = 0;
				
				return KICK;
			}
			kickVel = glm::vec3(30,-10,0);
			kickVel.y = Bezier(vel_curveup,1,t/casttime)*kickVel.y*facing;
		}
				vel = kickVel;
	}

	if(t > casttime)
		deactivateHitbox(hitboxList[KICKBOX]);

	
	
	t+=1;
	return KICK;
}
Actions PlayerActions::laserAction(int &t, glm::vec3 &vel, int facing, std::vector<Hitbox> &hitboxList){
	int vel_curve[4] = {0.5,1,0.5,0};
	int casttime = 40;
	int recovery = 90;

	deactivateAllHitbox(hitboxList);
	if(t > casttime+recovery)
		return IDLE;
	
	int laserVel = 5;

	if(t<casttime){
		activateHitbox(hitboxList[LASERBOX]);
		vel.y = 0;
		vel.x = Bezier(vel_curve,1,t/casttime)*laserVel*facing;
	}

	if(t>casttime)
		deactivateHitbox(hitboxList[LASERBOX]);

	

	t+=1;
	return LASER;
}
Actions PlayerActions::blastAction(int &t, glm::vec3 &vel, std::vector<Hitbox> &hitboxList){
	int casttime = 20;
	int recovery = 30;

	deactivateAllHitbox(hitboxList);
	if(t > casttime+recovery)
		return IDLE;

	if(t < casttime){
		activateHitbox(hitboxList[BLASTBOX]);
		vel.x = 0;
		vel.y = 0;
	}

	if(t > casttime)
		deactivateHitbox(hitboxList[BLASTBOX]);

	

	t+=1;
	return BLAST;
}

Actions PlayerActions::blockAction(int &t, glm::vec3 &vel, std::vector<Hitbox> &hitboxList, int &sp){
	
	deactivateAllHitbox(hitboxList);

	//if not enough sp left then can't block
	if(sp < 20)
		return IDLE;

	//stop movement while blocking
	vel.x = 0;
	vel.y = 0;

	//update timer, reduce sp
	t+=1;
	sp -= 20;

	return BLOCK;
}

Actions PlayerActions::staggerGAction(int &t, glm::vec3 &vel, int facing, std::vector<Hitbox> &hitboxList){
	int vel_curve[4] = {1,0.8,0.5,0};
	int casttime = 5;
	int recovery = 15;

	deactivateAllHitbox(hitboxList);
	if(t > casttime+recovery)
		return IDLE;

	int staggerGVel = 10;

	if(t < casttime)
		vel.x = Bezier(vel_curve,1,t/casttime)*staggerGVel*facing;
	
	t+=1;
	return STAGGER_G;
}

Actions PlayerActions::staggerAAction(int &t, glm::vec3 &vel, int facing, std::vector<Hitbox> &hitboxList){
	int vel_curveX[4] = {1,0.4,0.4,0};
	int vel_curveY[4] = {};

	int casttime = 5;
	int recovery = 15;

	int staggerAVelX = 40;
	int staggerAVelY = -5;

	deactivateAllHitbox(hitboxList);
	if(t > casttime+recovery)
		return IDLE;
	
	if(t < casttime){
		vel.x = Bezier(vel_curveX,1,t/casttime)*staggerAVelX*facing;
		vel.y = Bezier(vel_curveX,1,t/casttime)*staggerAVelY;
	}

	

	t+=1;
	return STAGGER_A;
}

Actions PlayerActions::idleAction(int &t, std::vector<Hitbox> &hitboxList){
	t = 0;
	deactivateAllHitbox(hitboxList);
	return IDLE;
}

