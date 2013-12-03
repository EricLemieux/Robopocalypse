#ifndef PLAYER_H
#define PLAYER_H

class Player{
public:
	Player();
	Player(OBJModel object);
	~Player();

	
	//temporary, delete soon
	void increaseVel(glm::vec3 vt);
	//kill velocity
	void stopVelX();
	void stopVelY();
	
	//Update the players current position
	//also check for collisions here.
	void update(Assets &assets, int playerIDNum,float t, Player &otherPlayer, NodeGraph &world_graph);
	void updatePos(float t);
	void updateWorldGraph(NodeGraph &world_graph);
    void updateCollision( Assets &assets,int playerIDNum, Player &otherPlayer,float t);

	//*********************************
	//these should be called in UserInput.cpp
	//player input decides what action happens
	void updateAction(int numAction);

	//update movement vars + any collisions
	void moveAction(int numAction);
	//attack choice, play animation + move if necessary, 
	void attackAction(int numAction);
	void updateAttack();
	//dodge, invincibility frames, animate, move, block input for set time
	void dodgeAction(int numAction);
	//block, reduce speed, block input for set time, set blocking status, animate
	void blockAction(int numAction);
	//***********************************

	//Stuff for A* shortest path
	std::vector<CollisionNode*> rangeAStar(Player &otherPlayer, NodeGraph &world_graph);
	float distanceBetween(CollisionNode* a, CollisionNode* b);
	std::vector<CollisionNode*> reconstructPath( CollisionNode* current_node);

	//Cooldown reduction
	void updateCooldown();

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

	inline int getFaceDirection(void){	return faceDirection;	}

	//velocity
	glm::vec3 getVelocity();
	void setVelocity(glm::vec3 newvelocity);

	float getVelX();
	float getVelY();
	float getVelZ();

	void setVelX();
	void setVelY();
	void setVelZ();

	//health and shield
	int getHealth();
	int getShield();

	inline void setHealth(int newHealth){ health = newHealth; }
	void setShield();

	//Stun cooldown
	inline void setStunCooldown(int cooldown){ stunCooldown = cooldown; }
	inline int getStunCooldown(){ return stunCooldown; }

	Path rangeAttackPath;

	void Death();

	//force
	glm::vec3 getTotalForce();

	//Collision boxes for different attacks
	collisionObjects attackFist;
	collisionObjects attackKick;
	collisionObjects attackRange;
	collisionObjects blockBox;


private:
	OBJModel playerObject;
	//collisionObjects playerHitBox;
	int input;

	//Variables for A* shortest path
	CollisionNode* targetNode;
	Player *internalOtherPlayer;
	NodeGraph *internalWorldgraph;

	//hackyshit get rid of this asap
	float floorPos;
	float leftWallPos;
	float rightWallPos;
	float floorSize;
	float leftWallSize;
	float rightWallSize;

	//glm::vec3 position;
	glm::vec3 position;
	glm::vec3 prevpos;
	glm::vec3 rotation;
	//glm::vec3 velocity;
	glm::vec3 velocity;
	float maxVelY;
	glm::vec3 acceleration;
	glm::vec3 totalForce;
	//force from collision with another player/attack
	glm::vec3 pushForce;
	glm::vec3 impactForce;
	//gravity
	glm::vec3 gravityForce;
	glm::vec3 moveForce;
	glm::vec3 resistanceForce;
	glm::vec3 jumpForce;

	
	
	//check for is moving, used to reset movement if button isn't held
	int isMoving;
	int isDashing;

	//check for whether player input is accepted
	int onCooldown;
	int blockCooldown;
    int rangeCooldown;
	//track current action
	int currentAction;
	//tracking for double jumps
	int jumpCount;
	//getting hit
	int stunCooldown;
	int wasHit;
	int invincibleFrames;
	//for bodybody collision
	int bodyTobody;

	//booster cooldown for dashing/jumping
	int boosterCooldown;
	int jumpCooldown;
	
	int isAttacking;
	int isKicking;
	glm::vec3 attackStartPos;
	float dt;
	//using this for temp speed control, put actual speed control later
	float tInterval;
	
	int isBlocking;

	//booleans for the players collision with the level geometry
	//On the Y axis
	bool hitFloor;
	bool hitCeiling;

	//On the X axis
	bool hitWall;

	//particle manager
	ParticleManager particlemanager;

	void checkOtherPlayer(Player &otherPlayer);
	//left is -1, right is 1
	int faceDirection;

	int health;
	int shield;

	//missile path
    Path rangePath;
};



#endif