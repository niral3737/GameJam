#ifndef _SOUND_MANAGER_
#define _SOUND_MANAGER_

#include <map>
#include "console.h"

struct sSoundInfo
{
	FMOD::Sound* _sound;
	FMOD::Channel* _channel;
	float channelPan = 0.0f;
};

class cSoundManager
{
public:
	static cSoundManager* getInstance();
	Console _console;
	bool _should_exit = false;

	FMOD_RESULT _result = FMOD_OK;
	FMOD::System *_system = NULL;
	/*FMOD::Sound* _sound[10];
	FMOD::Channel* _channel[10];*/
	//sSoundInfo soundInfo[10];
	bool wantCompressed;
	std::map<std::string, sSoundInfo*> mapSounds;
	sSoundInfo* selectedSound;
	~cSoundManager();

	bool initFmod();
	bool shutdownFmod();

	void changeBackgroundMusic();
	std::string getSoundFilePath(std::string key);
	void askForCompressedFiles();

	void playBackgroundMusic();

	void loadSFX();
	void playSFX(std::string soundName);
	void createAllSounds();

	void printInfo();

	void increaseVolume();
	void decreaseVolume();
	void increasePan();
	void decreasePan();
	void increasePitch();
	void decreasePitch();
	void increasePlabackSpeed();
	void decreasePlaybackSpeed();

private:
	cSoundManager();
	static cSoundManager* soundManager;
};

#endif


