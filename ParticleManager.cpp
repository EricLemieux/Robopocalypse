#include "ParticleManager.h"

ParticleManager::ParticleManager(){

}

ParticleManager::~ParticleManager(){

}

void ParticleManager::AddSmokeEmitter(){
	SmokeEmitter temp;
	smokeList.push_back(temp);
}
void ParticleManager::AddFireEmitter(){
	FireEmitter temp;
	fireList.push_back(temp);
}
void ParticleManager::AddSparkEmitter(){
	SparkEmitter temp;
	sparkList.push_back(temp);
}
void ParticleManager::AddImpactEmitter(){
	ImpactEmitter temp;
	impactList.push_back(temp);
}

void ParticleManager::update(float playerVel){
	for(int i = 0, size = smokeList.size(); i < size; ++i){
		smokeList[i].update(playerVel);
	}
	for(int i = 0, size = fireList.size(); i < size; ++i){
		fireList[i].update();
	}
	for(int i = 0, size = sparkList.size(); i < size; ++i){
		sparkList[i].update();
	}
	for(int i = 0, size = impactList.size(); i < size; ++i){
		impactList[i].update();
	}
}