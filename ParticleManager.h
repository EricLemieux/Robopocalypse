#pragma once

#include <vector>
#include "ParticleEmitter.h"

class ParticleManager{
private:
	std::vector<ParticleEmitter*> *emitterList;
	std::vector<ParticleType> typeList;
public:
	ParticleManager();
	~ParticleManager();

	void addEmitter(Node* parent,ParticleType type);

	int update();

	std::vector<ParticleEmitter*>* getEmitterList(){return emitterList;}
	std::vector<ParticleType> getType(){return typeList;}
};