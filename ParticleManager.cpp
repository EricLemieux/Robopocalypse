#include "ParticleManager.h"

ParticleManager::ParticleManager(){
	//emitterList = new std::vector<ParticleEmitter*>;
}

ParticleManager::~ParticleManager(){}

void ParticleManager::addEmitter(Node *parent, ParticleType type){
	emitterList.push_back(new ParticleEmitter);
	typeList.push_back(type);
	parent->AttachNode(emitterList.back()->GetNode());
	int a = 0;
}

int ParticleManager::update(Player *player1, Player *player2){
	for(int i = 0, size = emitterList.size(); i<size; ++i){
		emitterList.at(i)->update(typeList[i],player1,player2);
	}

	return 0;
}