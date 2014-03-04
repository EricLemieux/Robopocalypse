//written by Brendan Ho
#include "Sound.h"

void ERRCHECK(FMOD_RESULT result){
	if(result != FMOD_OK){
		//std::cout<<"oh shit, summin broke\n";
	}
}

SoundWrap::SoundWrap(){
	updateTime = 50;//ms
	distanceFactor = 1.f;

	flip = 0;
	mode = 0;

	isLogroll = 0;

	//set channel to 0
	channel1 = 0;
	channel2 = 0;

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



void SoundWrap::loadSound(std::string filename, int isLooping, FMOD_VECTOR soundPos, FMOD_VECTOR soundVel){
	FMOD::Sound *temp;

	result = system->createSound(filename.c_str(),FMOD_3D,0,&temp);
	ERRCHECK(result);


	result = temp->set3DMinMaxDistance(0.5f*distanceFactor, 5000.f*distanceFactor);
	ERRCHECK(result);

	result = temp->setMode(FMOD_LOOP_OFF);
	ERRCHECK(result);

	
	posList.push_back(soundPos);
	velList.push_back(soundVel);
	sfxList.push_back(temp);
	sfxLooping.push_back(isLooping);
}


void SoundWrap::clearAllSound(){
	for(int i = 0;i < sfxList.size(); ++i){
		result = sfxList[i]->release();
		ERRCHECK(result);
	}
}

void SoundWrap::set3DSettings(){
	if(isLogroll == 0){
		channel1->setMode(FMOD_3D_INVERSEROLLOFF);
		channel2->setMode(FMOD_3D_INVERSEROLLOFF);
		isLogroll = 1;
	} else {
		channel1->setMode(FMOD_3D_LINEARROLLOFF);
		channel2->setMode(FMOD_3D_LINEARROLLOFF);
		isLogroll = 0;
	}
}

void SoundWrap::linearMove(int soundNum){
	if(flip == 0){
		posList[soundNum].z += 0.5;
	} else {
		posList[soundNum].z -= 0.5;
	}

	if(posList[soundNum].z > 15){
		flip = 1;
	} else if (posList[soundNum].z < 1){
		flip = 0;
	}
}

void SoundWrap::circularPan(int soundNum){
	angle += 5;
	
	posList[soundNum].z = glm::sin(glm::radians(angle))*5;
	posList[soundNum].x = glm::cos(glm::radians(angle))*5;
	
}

int SoundWrap::playSound(int soundNum, int channelNum){
	if(channelNum == 1){
		//result = channel1->setPaused(false);
		ERRCHECK(result);
		//result = channel2->setPaused(true);
		ERRCHECK(result);
		result = system->playSound(FMOD_CHANNEL_FREE, sfxList[soundNum], false, &channel1);
		ERRCHECK(result);
		mode = 1;
		angle = 0;
		FMOD_VECTOR temp = {0,0,4};
		posList[soundNum] = temp;
		result = channel1->set3DAttributes(&posList[soundNum], &velList[soundNum]);
        ERRCHECK(result);
        
        
	}
	if(channelNum == 2){
		//result = channel2->setPaused(false);
		//std::cout<<"3333:  ";
		ERRCHECK(result);
		//result = channel1->setPaused(true);
		//std::cout<<"2222:  ";
		ERRCHECK(result);
		result = system->playSound(FMOD_CHANNEL_FREE, sfxList[soundNum], false, &channel2);
		//std::cout<<"1111:  ";
		ERRCHECK(result);
		mode = 2;
		FMOD_VECTOR temp = {0,0,2};
		posList[soundNum] = temp;
		result = channel2->set3DAttributes(&posList[soundNum], &velList[soundNum]);
		//std::cout<<"0000:  ";
        ERRCHECK(result);
       
	}
	return flip;
}

int SoundWrap::updateSound(){
	system->update();

	if(mode == 1){
		//circularPan(0);
		result = channel1->set3DAttributes(&posList[0], &velList[0]);
        ERRCHECK(result);
	} else if (mode == 2){
		//linearMove(0);
		result = channel2->set3DAttributes(&posList[0], &velList[0]);
        ERRCHECK(result);
	} else {
	
	}
    
	unsigned int ms = 0;
    unsigned int lenms = 0;
    bool         playing = 0;
    bool         paused = 0;
    int          channelsplaying = 0;

    if (channel1)
    {
        FMOD::Sound *currentsound = 0;

        result = channel1->isPlaying(&playing);
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
        {
            ERRCHECK(result);
        }

        result = channel1->getPaused(&paused);
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
        {
            ERRCHECK(result);
        }

        result = channel1->getPosition(&ms, FMOD_TIMEUNIT_MS);
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
        {
            ERRCHECK(result);
        }
       
        channel1->getCurrentSound(&currentsound);
        if (currentsound)
        {
            result = currentsound->getLength(&lenms, FMOD_TIMEUNIT_MS);
            if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
            {
                ERRCHECK(result);
            }
        }
    } else
	
	if (channel2)
    {
        FMOD::Sound *currentsound = 0;

        result = channel2->isPlaying(&playing);
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
        {
            ERRCHECK(result);
        }

        result = channel2->getPaused(&paused);
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
        {
            ERRCHECK(result);
        }

        result = channel2->getPosition(&ms, FMOD_TIMEUNIT_MS);
        if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
        {
            ERRCHECK(result);
        }
       
        channel2->getCurrentSound(&currentsound);
        if (currentsound)
        {
            result = currentsound->getLength(&lenms, FMOD_TIMEUNIT_MS);
            if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
            {
                ERRCHECK(result);
            }
        }
    }

    system->getChannelsPlaying(&channelsplaying);

	if(mode == 1)
		return angle;
	if(mode == 2)
		return flip;

	return 0;

    //std::cout<<"Time %02d:%02d:%02d/%02d:%02d:%02d : %s : Channels Playing %2d\r", ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100, lenms / 1000 / 60, lenms / 1000 % 60, lenms / 10 % 100, paused ? "Paused " : playing ? "Playing" : "Stopped", channelsplaying;
	//std::cout<<std::endl;
	

}