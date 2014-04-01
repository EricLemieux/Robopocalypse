#pragma once

#include "GameObject.h"
#include "SceneGraph.h"
#include "Player.h"

enum ParticleType{
	BASIC = 0,
	SMOKE,
	FIRE,
	SPARK,
	IMPACT,
	SHIELD,
};

class Particle : public GameObject{
private:

	glm::vec3 pos;
	glm::vec3 vel;

	Node* particleNode;

	VertexBuffer* VBO;

	int currentLife;
	int lifeSpan;

	glm::vec3 prevSmokePos;
	glm::vec3 currentSmokePos;

	bool flipped;

	bool isActive;

public:
	Particle();
	Particle(ParticleType type);
	~Particle();

	void Init(ParticleType type);
	void Deactivate();

	bool update(ParticleType type, Node* emitterNode, Player *player1, Player *player2);

	void release();

	Node* getParticleNode(){return particleNode;}
	bool isAlive(){return isActive;}
};