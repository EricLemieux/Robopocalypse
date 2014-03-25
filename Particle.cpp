#include "Particle.h"

Particle::Particle(ParticleType type){
	particleNode = this->GetNode();
	pos = glm::vec3(0,0,0);
	vel = glm::vec3(0,0,0);

	VBO = new VertexBuffer;
	VBO->Initialize(1,false,false,false);

	this->AttachModel(VBO);

	currentLife = 0;
	if(type == BASIC){
		lifeSpan = 10;
		this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/Sparks_1.png"));
		
	} else if(type == SMOKE){
		pos = glm::vec3(0,0,(float)(rand()%1-2));
		vel = glm::vec3((float)(rand()%3-1),(float)(rand()%2+1),0);
		lifeSpan = rand()%30 + 10;
		int temp = rand()%5+1;
		switch(temp){
		case 1:
			this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/Smoke_1.png"));
			break;
		case 2:
			this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/Smoke_2.png"));
			break;
		case 3:
			this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/Smoke_3.png"));
			break;
		case 4:
			this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/Smoke_4.png"));
			break;
		case 5:
			this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/Smoke_5.png"));
			break;
		default:
			this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/transparent.png"));
		}

	} else if(type == FIRE){
	
	} else if(type == SPARK){
		pos = glm::vec3(0,0,(float)(rand()%1-2));
		lifeSpan = 0;
		int temp = rand()%9+1;
		switch(temp){
		case 1:
			this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/Sparks_1.png"));
			break;														   
		case 2:															   
			this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/Sparks_2.png"));
			break;														   
		case 3:															   
			this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/Sparks_3.png"));
			break;														   
		case 4:															   
			this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/Sparks_4.png"));
			break;														   
		case 5:															   
			this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/Sparks_5.png"));
			break;														   
		case 6:															   
			this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/Sparks_6.png"));
			break;														   
		case 7:															   
			this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/Sparks_7.png"));
			break;														   
		case 8:															   
			this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/Sparks_8.png"));
			break;														   
		case 9:															   
			this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/Sparks_9.png"));
			break;
		default:
			this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/transparent.png"));
		}
	} else if(type == IMPACT){
	
	} else if (type == SHIELD){
		lifeSpan = 0;
		this->AttachTexture(loadAlphaTex("Resources/Textures/Particles/Shield.png"));
	}

	particleNode->SetLocalPosition(pos);
	prevSmokePos = particleNode->GetLocalPosition();
	currentSmokePos = particleNode->GetLocalPosition();
	flipped = false;

	int a = 0;
}

Particle::~Particle(){this->release();}

bool Particle::update(ParticleType type, Node* emitterNode){
	if(currentLife > lifeSpan)
		return false;
	
	pos = particleNode->GetLocalPosition();

	if(currentLife == 0){
		currentSmokePos = emitterNode->GetWorldPosition();
		prevSmokePos = currentSmokePos;
	} else {
		currentSmokePos = emitterNode->GetWorldPosition();
	}

	float difference =  (prevSmokePos.x - currentSmokePos.x);
	
	if(type == BASIC){
		pos.y += 1.f;
	} else if(type == SMOKE){
		
		pos.z += (vel.x*0.2f) - difference;
		pos.y += vel.y*0.5f;
		vel.x *= 0.9f;

		//if(difference>0){
		//	flipped = !flipped;
		//}
		//if(flipped){
		//	pos.z = -1*pos.z;
		//}
	} else if(type == FIRE){
	
	} else if(type == SPARK){
		pos = glm::vec3((float)(rand()%6-3),(float)(rand()%15 - 7),(float)(rand()%6-3));
	} else if(type == IMPACT){
	
	} else if (type == SHIELD){
		
	}

	prevSmokePos = currentSmokePos;
	++currentLife;


	particleNode->SetLocalPosition(pos);
	
	
	return true;
}

void Particle::release(){
	delete(particleNode);
	delete(VBO);
}