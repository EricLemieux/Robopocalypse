//written by Brendan Ho
#include "Sound.h"

//for debugging
void ERRCHECK(FMOD_RESULT result){
	if(result != FMOD_OK){
		//std::cout<<"oh shit, summin broke\n";
	}
}

SoundWrap::SoundWrap(){
	updateTime = 50;//ms
	distanceFactor = 1.f;

	//set channel to 0
	for(int i = 0; i < 5; ++i){
		FMOD::Channel *temp;
		channelList.push_back(temp);
	}

	

	listenerflag = true;
	FMOD_VECTOR tempFMODVEC = { 0.0f, 0.0f, -1.0f * distanceFactor };
	listenerpos  = tempFMODVEC;

	//declare system
	result = FMOD::System_Create(&system);
    ERRCHECK(result);

	//check version
    result = system->getVersion(&version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        std::cout<<"wrong version of fmod yo\n";
    }

	result = system->getNumDrivers(&numdrivers);
    ERRCHECK(result);
	if (numdrivers == 0)
    {
        result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
        ERRCHECK(result);
    }
    else
    {
        result = system->getDriverCaps(0, &caps, 0, &speakermode);
        ERRCHECK(result);

        result = system->setSpeakerMode(speakermode);       /* Set the user selected speaker mode. */
        ERRCHECK(result);

        if (caps & FMOD_CAPS_HARDWARE_EMULATED)             /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
        {                                                   /* You might want to warn the user about this. */
            result = system->setDSPBufferSize(1024, 10);
            ERRCHECK(result);
        }

        result = system->getDriverInfo(0, name, 256, 0);
        ERRCHECK(result);

        if (strstr(name, "SigmaTel"))   /* Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it. */
        {
            result = system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
            ERRCHECK(result);
        }
    }

	//init with max 100 channels
    result = system->init(100, FMOD_INIT_NORMAL, 0);
    if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)         //default to stereo if selected speaker mode is not supported
    {
        result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
        ERRCHECK(result);
            
        result = system->init(100, FMOD_INIT_NORMAL, 0);
        ERRCHECK(result);
    }

	//setting distance units
	result = system->set3DSettings(1.0, distanceFactor, 1);
    ERRCHECK(result);


}
SoundWrap::~SoundWrap(){
	clearAllSound();
	result = system->close();
	ERRCHECK(result);
	result = system->release();
	ERRCHECK(result);
}



void SoundWrap::loadSound(const char *filename){
	
	char *firstWord = new char();
	char *data = new char();

	std::ifstream file;
	file.open(filename);
	if(!file.is_open()){
		std::cout << "Error opening the file " << filename << "\n";
		return;
	}

	while (!file.eof()){
		FMOD::Sound *temp;
		file >> firstWord;

		if(!_stricmp(firstWord, "psfx")){
			char psfxPath[256];
			file >> psfxPath;
			system->createSound(psfxPath, FMOD_3D,0,&temp);
			temp->set3DMinMaxDistance(0.5f*distanceFactor, 5000.f*distanceFactor);
			temp->setMode(FMOD_LOOP_OFF);
			playerSFXList.push_back(temp);
			file.ignore(256, '\n');

		}
		else if (!_stricmp(firstWord, "msfx")){
			char msfxPath[256];
			file >> msfxPath;
			system->createSound(msfxPath, FMOD_3D,0,&temp);
			temp->set3DMinMaxDistance(0.5f*distanceFactor, 5000.f*distanceFactor);
			temp->setMode(FMOD_LOOP_OFF);
			menuSFXList.push_back(temp);
			file.ignore(256, '\n');
		}
		else if (!_stricmp(firstWord, "bgm")){
			char bgmPath[256];
			file >> bgmPath;
			system->createSound(bgmPath, FMOD_3D,0,&temp);
			temp->set3DMinMaxDistance(0.5f*distanceFactor, 5000.f*distanceFactor);
			temp->setMode(FMOD_LOOP_NORMAL);
			BGMList.push_back(temp);
			file.ignore(256, '\n');
		}
	}

}


void SoundWrap::clearAllSound(){
	for(int i = 0;i < playerSFXList.size(); ++i){
		result = playerSFXList[i]->release();
		ERRCHECK(result);
	}
	for(int i = 0;i < menuSFXList.size(); ++i){
		result = menuSFXList[i]->release();
		ERRCHECK(result);
	}
	for(int i = 0;i < BGMList.size(); ++i){
		result = BGMList[i]->release();
		ERRCHECK(result);
	}
}


void SoundWrap::setChannelPos(int chanNum, glm::vec3 pos){
	FMOD_VECTOR temp = {pos.x,pos.y,pos.z};
	FMOD_VECTOR temp2 = {0,0,0};
	//channelList[chanNum]->set3DAttributes(&temp,&temp2);
}

void SoundWrap::pauseSound(int channelNum){
	channelList[channelNum]->setPaused(true);
}

void SoundWrap::playSound(int soundNum, int channelNum){
	channelList[channelNum]->setPaused(false);
	if(channelNum == BGM_FIGHT_CHANNEL){
		system->playSound(FMOD_CHANNEL_FREE, BGMList[soundNum], true, &channelList[channelNum]);
		channelList[channelNum]->setVolume(0.2f);
		channelList[channelNum]->setPaused(false);
		//system->playSound(FMOD_CHANNEL_FREE, BGMList[0], false, &channelList[channelNum]);
	} else
	if(channelNum == BGM_VICTORY_CHANNEL){
		//system->playSound(FMOD_CHANNEL_FREE, BGMList[soundNum], false, &channelList[channelNum]);
		//system->playSound(FMOD_CHANNEL_FREE, BGMList[0], false, &channelList[channelNum]);	
	} else
	if(channelNum == SFX_PLAYER1_CHANNEL){
		system->playSound(FMOD_CHANNEL_FREE, playerSFXList[soundNum], false, &channelList[channelNum]);
		//system->playSound(FMOD_CHANNEL_FREE, playerSFXList[0], false, &channelList[channelNum]);
	} else
	if(channelNum == SFX_PLAYER2_CHANNEL){
		system->playSound(FMOD_CHANNEL_FREE, playerSFXList[soundNum], false, &channelList[channelNum]);
		//system->playSound(FMOD_CHANNEL_FREE, playerSFXList[0], false, &channelList[channelNum]);
	} else
	if(channelNum == SFX_OTHER_CHANNEL){
		system->playSound(FMOD_CHANNEL_FREE, menuSFXList[soundNum], false, &channelList[channelNum]);
		//system->playSound(FMOD_CHANNEL_FREE, menuSFXList[0], false, &channelList[channelNum]);
	}
	
}

int SoundWrap::updateSound(){
	system->update();
    int          channelsplaying = 0;
    system->getChannelsPlaying(&channelsplaying);

	return 0;
	

}