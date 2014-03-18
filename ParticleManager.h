#pragma once

#include "ParticleEmitters.h"

class ParticleManager{
private:
	std::vector<SmokeEmitter> smokeList;
	std::vector<FireEmitter> fireList;
	std::vector<SparkEmitter> sparkList;
	std::vector<ImpactEmitter> impactList;
public:
	ParticleManager();
	~ParticleManager();

	void AddSmokeEmitter();
	void AddFireEmitter();
	void AddSparkEmitter();
	void AddImpactEmitter();

	void update(float playerVel);
};