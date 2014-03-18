#pragma once

#include "Particles.h"

class ParticleEmitter : public GameObject{
public:
	Node *emitterNode;
	 glm::vec3 pos;
	 glm::vec3 vel;

	 bool isActive;
	 int timer;

	ParticleEmitter();
	~ParticleEmitter();

	void update();

	void Activate();
	void Deactivate();
};

class SmokeEmitter : public ParticleEmitter{
private:
	std::vector<SmokeParticle> particleList;
public:
	SmokeEmitter();
	~SmokeEmitter();

	void update(float playerVel);
};

class FireEmitter : public ParticleEmitter{
private:
	std::vector<FireParticle> particleList;
public:
	FireEmitter();
	~FireEmitter();

	void update();
};

class SparkEmitter : public ParticleEmitter{
private:
	std::vector<SparkParticle> particleList;
	int nextSpark;
public:
	SparkEmitter();
	~SparkEmitter();

	void update();
};

class ImpactEmitter : public ParticleEmitter{
private:
	std::vector<ImpactParticle> particleList;
public:
	ImpactEmitter();
	~ImpactEmitter();

	void update();
};