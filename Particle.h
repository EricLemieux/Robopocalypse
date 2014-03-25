#pragma once

#include "GameObject.h"
#include "SceneGraph.h"

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

public:
	Particle(ParticleType type);
	~Particle();

	bool update(ParticleType type, Node* emitterNode);

	void release();

	Node* getParticleNode(){return particleNode;}
};