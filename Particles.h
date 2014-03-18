#pragma once

#include <vector>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "GameObject.h"
#include "SceneGraph.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>



class Particle : public GameObject{
public:
	glm::vec3 pos;
	glm::vec3 vel;
	Node *particleNode;
	float life;
	float currentLife;

	Particle();
	~Particle();

	void update();

	bool isAlive();
};

class SmokeParticle : public Particle{
private:
	float initXVel;
public:
	SmokeParticle(float playerVel);
	~SmokeParticle();


	void update();
};

class FireParticle : public Particle{
private:

public:
	FireParticle();
	~FireParticle();

	void update();
};

class SparkParticle : public Particle{
public:
	SparkParticle();
	~SparkParticle();

	void update();
};

class ImpactParticle : public Particle{
private:
	int orientation;
public:
	ImpactParticle();
	~ImpactParticle();

	void update();
};