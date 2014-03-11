#include "PlayerActions.h"

PlayerActions::PlayerActions(){}
PlayerActions::~PlayerActions(){}


void PlayerActions::activateHitbox(CollisionBox &box){
	box.SetIsActive(true);
	glm::vec3 pos = box.GetSceneGraphObject()->GetLocalPosition();
	box.GetSceneGraphObject()->SetLocalPosition(glm::vec3(pos.x, pos.y, 0.0f));
}
void PlayerActions::deactivateHitbox(CollisionBox &box){
	box.SetIsActive(false);
	glm::vec3 pos = box.GetSceneGraphObject()->GetLocalPosition();
	box.GetSceneGraphObject()->SetLocalPosition(glm::vec3(pos.x, pos.y, 1000.0f));
}

//doesn't deactivate body hitbox
void PlayerActions::deactivateAllHitbox(std::vector<CollisionBox> &hitboxList){
	deactivateHitbox(hitboxList[PUNCHBOX]);
	deactivateHitbox(hitboxList[KICKBOX]);
	deactivateHitbox(hitboxList[LASERBOX]);
	deactivateHitbox(hitboxList[BLASTBOX]);
}

Actions PlayerActions::moveLeftAction(int &t, glm::vec3 &vel){
	

	int moveVel = -50;

	//if speed is less than max, increase it otherwise reduce it
	if(vel.x < moveVel)
		vel.x += 5;
	else
		vel.x -= 5;

	t+=1;
	
	return IDLE;
}

Actions PlayerActions::moveRightAction(int &t, glm::vec3 &vel){
	
	int moveVel = 50;

	if(vel.x > moveVel)
		vel.x -= 5;
	else
		vel.x += 5;

	t+=1;
	
	return IDLE;
}

Actions PlayerActions::dashLeftAction(int &t, glm::vec3 &vel, int &sp){

	int dashVel = -100;

	if (sp > 50){
		vel.x = dashVel;
		sp -= 100;
	}

	return IDLE;
}

Actions PlayerActions::dashRightAction(int &t, glm::vec3 &vel, int &sp){

	int dashVel = 100;

	if (sp > 50){
		vel.x = dashVel;
		sp -= 100;
	}

	return IDLE;
}

//TODO: Ground
Actions PlayerActions::jumpAction(glm::vec3 &vel, int onGround){
	int jumpVel = 80;

	if(onGround == 1)
		vel.y = jumpVel;

	return IDLE;
}

Actions PlayerActions::punchAction(int &t, glm::vec3 &vel, int facing, std::vector<CollisionBox> &hitboxList, int onGround){
	float vel_curve[4] = { 0, 0.5, 0.8, 1 };

	float casttime = 10;
	float recovery = 30;

	float punchVel = 150;

	deactivateAllHitbox(hitboxList);
	if (t > (casttime + recovery))
		return IDLE;

	if (t<casttime){
		activateHitbox(hitboxList[PUNCHBOX]);
		glm::vec3 pos = hitboxList[PUNCHBOX].GetSceneGraphObject()->GetLocalPosition();
		hitboxList[PUNCHBOX].GetSceneGraphObject()->SetLocalPosition(glm::vec3(0.f, 4.f, (4 + (4 * (float)t / casttime))));

		int x = Bezier(vel_curve, 1, (float)t / casttime);
		vel.x = Bezier(vel_curve, 1, (float)t / casttime)*punchVel*facing;
		vel.y = 0;
	}

	if (t>casttime){
		deactivateHitbox(hitboxList[PUNCHBOX]);
		if (onGround == 1){
			vel.x = 0;
		}
		else {
			vel.x = vel.x*0.99f;
		}
	}

	t += 1;
	return PUNCH;
}


Actions PlayerActions::kickAction(int &t, glm::vec3 &vel, int facing, int onGround, std::vector<CollisionBox> &hitboxList){
	float vel_curveup[4] = { 1, 0.5, 0.8, 0 };
	float vel_curvedown[4] = { 1, 0.5, 0.8, 0 };
	glm::vec3 kickVel;
	float casttime = 20;
	float recovery = 40;

	deactivateAllHitbox(hitboxList);

	if (t>casttime + recovery)
		return IDLE;

	//initial check for starting point, if 0 starting from ground
	if ((t == 0) && (onGround == 1)){
		kickStart = 0;
	}
	else if ((t == 0) && (onGround == 0)){
		kickStart = 1;
	}


	if (t < casttime){
		activateHitbox(hitboxList[KICKBOX]);
		glm::vec3 pos = hitboxList[KICKBOX].GetSceneGraphObject()->GetLocalPosition();
		if (kickStart == 0){

			hitboxList[KICKBOX].GetSceneGraphObject()->SetLocalPosition(glm::vec3(0.f, -4.f + (8 * (float)t / casttime), (4 + (4 * (float)t / casttime))));

			kickVel = glm::vec3(30 * facing, 40, 0);
			kickVel.y = Bezier(vel_curvedown, 1, (float)t / casttime)*kickVel.y;
		}
		else if (kickStart == 1){ // diiiiive kiiiickuuuu
			//if hit the ground, stop

			hitboxList[KICKBOX].GetSceneGraphObject()->SetLocalPosition(glm::vec3(0.f, -4.f, (4 + (2 * (float)t / casttime))));

			if (onGround == 1){

				t = casttime + 1;
				vel.x = 0;
				vel.y = 0;

				return KICK;
			}

			kickVel = glm::vec3(60 * facing, -60, 0);
			kickVel.y = Bezier(vel_curveup, 1, (float)t / casttime)*kickVel.y;
		}
		vel = kickVel;
	}

	if (t > casttime){
		if (kickStart == 1 && onGround == 1 && t < (casttime + (recovery / 2.f))){
			hitboxList[KICKBOX].GetSceneGraphObject()->SetLocalPosition(glm::vec3(0.f, -4.f, 4));
			hitboxList[KICKBOX].GetCollisionBox()->GetSceneGraphObject()->SetScale(glm::vec3(20.f*((float)t / (casttime + recovery)), 5.f*((float)t / (casttime + recovery)), 10.f*((float)t / (casttime + recovery))));
		}
		else {
			deactivateHitbox(hitboxList[KICKBOX]);
			hitboxList[KICKBOX].GetCollisionBox()->GetSceneGraphObject()->SetScale(glm::vec3(1.f, 1.f, 1.f));

		}
		if (onGround == 0){
			vel.x = vel.x*0.95f;
		}
		else {
			vel.x = 0;
		}
	}


	t += 1;
	return KICK;
}
Actions PlayerActions::laserAction(int &t, glm::vec3 &vel, int facing, std::vector<CollisionBox> &hitboxList){
	float vel_curve[4] = { 0.5, 1, 0.5, 0 };
	float casttime = 10;
	float recovery = 60;

	deactivateAllHitbox(hitboxList);
	if (t > casttime + recovery)
		return IDLE;

	int laserVel = -15;

	if (t<casttime){
		activateHitbox(hitboxList[LASERBOX]);
		glm::vec3 pos = hitboxList[LASERBOX].GetSceneGraphObject()->GetLocalPosition();
		hitboxList[LASERBOX].GetSceneGraphObject()->SetLocalPosition(glm::vec3(0.f, 3.f,25.f));
		vel.y = 0;
		vel.x = Bezier(vel_curve, 1, (float)t / casttime)*laserVel*facing;
	}

	if (t>casttime){
		vel.x = 0;
		deactivateHitbox(hitboxList[LASERBOX]);
	}


	t += 1;
	return LASER;
}
Actions PlayerActions::blastAction(int &t, glm::vec3 &vel, std::vector<CollisionBox> &hitboxList){
	float casttime = 20;
	float recovery = 30;

	deactivateAllHitbox(hitboxList);
	if (t > casttime + recovery)
		return IDLE;

	if (t < casttime){
		activateHitbox(hitboxList[BLASTBOX]);
		hitboxList[BLASTBOX].GetCollisionBox()->GetSceneGraphObject()->SetScale(glm::vec3(30.f*((float)t / casttime), 20.f*((float)t / casttime), 10.f*((float)t / casttime)));
		vel.x = 0;
		vel.y = 0;
	}

	if (t > casttime)
		deactivateHitbox(hitboxList[BLASTBOX]);



	t += 1;
	return BLAST;
}


Actions PlayerActions::blockAction(int &t, glm::vec3 &vel, std::vector<CollisionBox> &hitboxList, int &sp){
	
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

	return IDLE;
}

Actions PlayerActions::staggerGAction(int &t, glm::vec3 &vel, int facing, std::vector<CollisionBox> &hitboxList, int &hasBeenHit){
	float vel_curve[4] = { 1, 0.8, 0.5, 0 };
	float casttime = 25;
	float recovery = 5;

	deactivateAllHitbox(hitboxList);
	if (t > casttime + recovery){
		hasBeenHit = 0;
		return IDLE;
	}


	int staggerGVel = -50;

	if (t < casttime)
		vel.x = Bezier(vel_curve, 1, (float)t / casttime)*staggerGVel*facing;


	t += 1;
	return STAGGER_G;
}

Actions PlayerActions::staggerAAction(int &t, glm::vec3 &vel, int facing, std::vector<CollisionBox> &hitboxList, int &hasBeenHit){
	float vel_curveX[4] = { 1, 0.4, 0.4, 0 };


	float casttime = 25;
	float recovery = 5;

	float staggerAVelX = -50;

	deactivateAllHitbox(hitboxList);

	if (t > casttime + recovery){
		hasBeenHit = 0;
		return IDLE;
	}

	hasBeenHit += 1;

	if (t < casttime){
		vel.x = Bezier(vel_curveX, 1, t / casttime)*staggerAVelX*facing;
		vel.y = 0;
	}

	t += 1;
	return STAGGER_A;
}

Actions PlayerActions::idleAction(int &t, std::vector<CollisionBox> &hitboxList){
	t = 0;
	deactivateAllHitbox(hitboxList);
	return IDLE;
}