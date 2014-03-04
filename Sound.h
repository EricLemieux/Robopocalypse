//written by Brendan Ho
#ifndef SOUNDWRAP_H
#define SOUNDWRAP_H

#include <fmod.hpp>
#include <fmod_errors.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <iostream>
#include <vector>

class SoundWrap{
private:
	

	int updateTime;//ms
	float distanceFactor;//per meter
	FMOD::System     *system;
	std::vector<FMOD::Sound*> sfxList;
	std::vector<FMOD_VECTOR> posList;
	std::vector<FMOD_VECTOR> velList;
	std::vector<int> sfxLooping;//0 no, 1 yes
    FMOD::Channel    *channel1, *channel2;
    FMOD_RESULT       result;
    int               key, numdrivers;
    unsigned int      version;
	bool             listenerflag;
    FMOD_VECTOR      listenerpos;
    FMOD_SPEAKERMODE speakermode;
    FMOD_CAPS        caps;
    char             name[256];

	//for assignment 1
	int mode;
	float angle;

public:
	
	SoundWrap();
	~SoundWrap();
	//void ERRCHECK(FMOD_RESULT result);
	
	void loadSound(std::string filename, int isLooping, FMOD_VECTOR soundPos, FMOD_VECTOR soundVel);
	//void deleteSound(int soundIndex);
	void clearAllSound();

	int isLogroll;
	void set3DSettings();


	int playSound(int soundNum, int channelNum);

	int flip;
	void linearMove(int soundNum);
	void circularPan(int soundNum);

	int updateSound();
};

#endif