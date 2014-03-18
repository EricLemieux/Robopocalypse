//written by Brendan Ho
#ifndef SOUNDWRAP_H
#define SOUNDWRAP_H

#include <fmod.hpp>
#include <fmod_errors.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <iostream>
#include <fstream>
#include <vector>

enum ChannelType{
	BGM_FIGHT_CHANNEL = 0,
	BGM_VICTORY_CHANNEL,
	SFX_PLAYER1_CHANNEL,
	SFX_PLAYER2_CHANNEL,
	SFX_OTHER_CHANNEL,
};

enum playerSFX{
	
	IMPACT1_SFX = 0,
	IMPACT2_SFX,
	IMPACT3_SFX,
	BLOCK_IMPACT_SFX,
	//MOVE_SFX,
	//DASH_SFX,
	//JUMP_SFX,
	//LAND_SFX,
	PUNCH_SFX,
	KICK_SFX,
	LASER_SFX,
	BLAST_SFX,
	BLOCK_UP_SFX,
	BLOCK_DOWN_SFX,
	EMPTY_P_SFX,
};

enum menuSFX{
	
	HIGHLIGHT_SFX = 0,
	SELECT_SFX,
	PAUSE_SFX,
	EMPTY_M_SFX,
};

class SoundWrap{
private:
	

	int updateTime;//ms
	float distanceFactor;//per meter
	FMOD::System     *system;
	std::vector<FMOD::Sound*> playerSFXList;
	std::vector<FMOD::Sound*> menuSFXList;
	std::vector<FMOD::Sound*> BGMList;
	std::vector<FMOD::Channel*> channelList;
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
	
	void loadSound(const char *filename);
	//void deleteSound(int soundIndex);
	void clearAllSound();

	int isLogroll;
	void set3DSettings();


	void playSound(int soundNum, int channelNum);
	void pauseSound(int channelNum);

	int flip;
	void linearMove(int soundNum);
	void circularPan(int soundNum);

	void setChannelPos(int channelNum, glm::vec3 pos);

	int updateSound();
};

#endif