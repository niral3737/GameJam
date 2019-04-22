#include "cSoundManager.h"

#include "json.hpp"
#include "cJsonUtils.h"

cSoundManager* cSoundManager::soundManager = NULL;

const unsigned int FILENAME_LENGTH = 50;
#define STREAM_BUFFER_SIZE 65536
std::map<FMOD_SOUND_FORMAT, std::string> mapFormat = {
	{FMOD_SOUND_FORMAT_NONE,             "Unitialized / unknown. "},
	{FMOD_SOUND_FORMAT_PCM8,             "8bit integer PCM data." },
	{FMOD_SOUND_FORMAT_PCM16,            "16bit integer PCM data." },
	{FMOD_SOUND_FORMAT_PCM24,            "24bit integer PCM data."},
	{FMOD_SOUND_FORMAT_PCM32,            "32bit integer PCM data." },
	{FMOD_SOUND_FORMAT_PCMFLOAT,         "32bit floating point PCM data." },
	{FMOD_SOUND_FORMAT_BITSTREAM,        "Sound data is in its native compressed format." },
	{FMOD_SOUND_FORMAT_MAX,              "Maximum number of sound formats supported."}
};

std::map<FMOD_SOUND_TYPE, std::string> mapType = {
	{FMOD_SOUND_TYPE_UNKNOWN,          "3rd party / unknown plugin format." },
	{FMOD_SOUND_TYPE_AIFF,             "AIFF." },
	{FMOD_SOUND_TYPE_ASF,              "Microsoft Advanced Systems Format (ie WMA/ASF/WMV)." },
	{FMOD_SOUND_TYPE_DLS,              "Sound font / downloadable sound bank." },
	{FMOD_SOUND_TYPE_FLAC,             "FLAC lossless codec." },
	{FMOD_SOUND_TYPE_FSB,              "FMOD Sample Bank." },
	{FMOD_SOUND_TYPE_IT,               "Impulse Tracker." },
	{FMOD_SOUND_TYPE_MIDI,             "MIDI." },
	{FMOD_SOUND_TYPE_MOD,              "Protracker / Fasttracker MOD." },
	{FMOD_SOUND_TYPE_MPEG,             "MP2/MP3 MPEG." },
	{FMOD_SOUND_TYPE_OGGVORBIS,        "Ogg vorbis." },
	{FMOD_SOUND_TYPE_PLAYLIST,         "Information only from ASX/PLS/M3U/WAX playlists "},
	{FMOD_SOUND_TYPE_RAW,             "Raw PCM data." },
	{FMOD_SOUND_TYPE_S3M,              "ScreamTracker 3." },
	{FMOD_SOUND_TYPE_USER,             "User created sound." },
	{FMOD_SOUND_TYPE_WAV,              "Microsoft WAV." },
	{FMOD_SOUND_TYPE_XM,               "FastTracker 2 XM." },
	{FMOD_SOUND_TYPE_XMA,              "Xbox360 XMA "},
	{FMOD_SOUND_TYPE_AUDIOQUEUE,       "iPhone hardware decoder, supports AAC, ALAC and MP3." },
	{FMOD_SOUND_TYPE_AT9,              "PS4 / PSVita ATRAC 9 format" },
	{FMOD_SOUND_TYPE_VORBIS,          "Vorbis" },
	{FMOD_SOUND_TYPE_MEDIA_FOUNDATION, "Windows Store Application built in system codecs" },
	{FMOD_SOUND_TYPE_MEDIACODEC,       "Android MediaCodec" },
	{FMOD_SOUND_TYPE_FADPCM,           "FMOD Adaptive Differential Pulse Code Modulation" },

	{FMOD_SOUND_TYPE_MAX,				 "Maximum number of sound types supported." },
};

cSoundManager * cSoundManager::getInstance()
{
	if (soundManager == NULL)
	{
		soundManager = new cSoundManager();
	}
	return soundManager;
}

cSoundManager::cSoundManager()
{
	this->wantCompressed = false;
	this->selectedSound = NULL;
}


cSoundManager::~cSoundManager()
{}

bool cSoundManager::initFmod()
{
	//Create the main system object
	_result = FMOD::System_Create(&_system);
	//TODO: CHECK FOR FMOD ERRORS, IMPLEMENT YOUR OWN FUNCTION
	assert(!_result);
	//Initializes the system object, and the msound device. This has to be called at the start of the user's program
	_result = _system->init(512, FMOD_INIT_NORMAL, NULL);
	assert(!_result);


	return true;
}

bool cSoundManager::shutdownFmod()
{
	/*for (size_t i = 0; i < 10; i++)
	{
		if (soundInfo[i]._sound)
		{
			_result = soundInfo[i]._sound->release();
			assert(!_result);
		}
	}*/
	for (std::map<std::string, sSoundInfo*>::iterator it = mapSounds.begin(); it != mapSounds.end(); it++)
	{
		if (it->second->_sound)
		{
			_result = it->second->_sound->release();
			assert(!_result);
		}
	}
	/*if (_sound)
	{
		_result = _sound->release();
		assert(!_result);
	}*/
	if (_system)
	{
		_result = _system->close();
		assert(!_result);
		_result = _system->release();
		assert(!_result);
	}

	return true;
}


std::string cSoundManager::getSoundFilePath(std::string key)
{
	nlohmann::json json = cJsonUtils::getJsonInstance();
	std::string basePath = json["soundsBasePath"].get<std::string>();
	std::string soundFileName = json[key].get<std::string>();
	std::string ext;

	if (this->wantCompressed)
		ext = json["compressedFileExt"].get<std::string>();
	else
		ext = json["uncompressedFileExt"].get<std::string>();


	return basePath + soundFileName + ext;
}

void cSoundManager::askForCompressedFiles()
{
	std::cout << "Play compressed files?(Y/N)" << std::endl;
	char answer;
	std::cin >> answer;
	this->wantCompressed = (answer == 'y');
}
void cSoundManager::createAllSounds()
{
	mapSounds["backgroundSound1"] = new sSoundInfo();
	_result = _system->createSound(getSoundFilePath("backgroundSound1").c_str(), FMOD_LOOP_NORMAL, 0, &mapSounds["backgroundSound1"]->_sound);
	assert(!_result);

	mapSounds["backgroundSound2"] = new sSoundInfo();
	_result = _system->createSound(getSoundFilePath("backgroundSound2").c_str(), FMOD_LOOP_NORMAL, 0, &mapSounds["backgroundSound2"]->_sound);
	assert(!_result);

	mapSounds["backgroundSound3"] = new sSoundInfo();
	_result = _system->createSound(getSoundFilePath("backgroundSound3").c_str(), FMOD_LOOP_NORMAL, 0, &mapSounds["backgroundSound3"]->_sound);
	assert(!_result);

	mapSounds["axeSound"] = new sSoundInfo();
	_result = _system->createSound(getSoundFilePath("axeSound").c_str(), FMOD_DEFAULT, 0, &mapSounds["axeSound"]->_sound);
	assert(!_result);

	mapSounds["appleSound"] = new sSoundInfo();
	_result = _system->createSound(getSoundFilePath("appleSound").c_str(), FMOD_DEFAULT, 0, &mapSounds["appleSound"]->_sound);
	assert(!_result);

	mapSounds["jumpSound"] = new sSoundInfo();
	_result = _system->createSound(getSoundFilePath("jumpSound").c_str(), FMOD_DEFAULT, 0, &mapSounds["jumpSound"]->_sound);
	assert(!_result);

	mapSounds["punchSound"] = new sSoundInfo();
	_result = _system->createSound(getSoundFilePath("punchSound").c_str(), FMOD_DEFAULT, 0, &mapSounds["punchSound"]->_sound);
	assert(!_result);

	mapSounds["splashSound"] = new sSoundInfo();
	_result = _system->createSound(getSoundFilePath("splashSound").c_str(), FMOD_DEFAULT, 0, &mapSounds["splashSound"]->_sound);
	assert(!_result);

	mapSounds["fireSound"] = new sSoundInfo();
	_result = _system->createSound(getSoundFilePath("fireSound").c_str(), FMOD_DEFAULT, 0, &mapSounds["fireSound"]->_sound);
	assert(!_result);

	mapSounds["forestSound"] = new sSoundInfo();
	_result = _system->createSound(getSoundFilePath("forestSound").c_str(), FMOD_DEFAULT, 0, &mapSounds["forestSound"]->_sound);
	assert(!_result);
}

void cSoundManager::playBackgroundMusic()
{
	this->_result = this->_system->playSound(mapSounds["backgroundSound1"]->_sound, 0, false, &mapSounds["backgroundSound1"]->_channel);
	assert(!this->_result);

	this->_result = this->_system->playSound(mapSounds["backgroundSound2"]->_sound, 0, false, &mapSounds["backgroundSound2"]->_channel);
	assert(!this->_result);

	this->_result = this->_system->playSound(mapSounds["backgroundSound3"]->_sound, 0, true, &mapSounds["backgroundSound3"]->_channel);
	assert(!_result);
}


void cSoundManager::changeBackgroundMusic()
{
	bool isPaused = false;
	_result = mapSounds["backgroundSound3"]->_channel->getPaused(&isPaused);
	assert(!_result);
	
	_result = mapSounds["backgroundSound3"]->_channel->setPaused(!isPaused);
	assert(!_result);
}

void cSoundManager::loadSFX()
{
	this->_result = this->_system->playSound(mapSounds["axeSound"]->_sound, 0, true, &mapSounds["axeSound"]->_channel);
	assert(!this->_result);

	this->_result = this->_system->playSound(mapSounds["appleSound"]->_sound, 0, true, &mapSounds["appleSound"]->_channel);
	assert(!this->_result);

	this->_result = this->_system->playSound(mapSounds["jumpSound"]->_sound, 0, true, &mapSounds["jumpSound"]->_channel);
	assert(!this->_result);

	this->_result = this->_system->playSound(mapSounds["punchSound"]->_sound, 0, true, &mapSounds["punchSound"]->_channel);
	assert(!this->_result);

	this->_result = this->_system->playSound(mapSounds["splashSound"]->_sound, 0, true, &mapSounds["splashSound"]->_channel);
	assert(!this->_result);

	this->_result = this->_system->playSound(mapSounds["fireSound"]->_sound, 0, true, &mapSounds["fireSound"]->_channel);
	assert(!this->_result);

	this->_result = this->_system->playSound(mapSounds["forestSound"]->_sound, 0, true, &mapSounds["forestSound"]->_channel);
	assert(!this->_result);
}

void cSoundManager::playSFX(std::string soundName)
{
	this->_result = this->_system->playSound(mapSounds[soundName]->_sound, 0, false, &mapSounds[soundName]->_channel);
	assert(!this->_result);
}



void cSoundManager::printInfo()
{
	if (selectedSound != NULL)
	{
		char fileName[FILENAME_LENGTH];
		FMOD_SOUND_FORMAT format = FMOD_SOUND_FORMAT_NONE;
		FMOD_SOUND_TYPE type = FMOD_SOUND_TYPE_UNKNOWN;
		float frequency = 0.0f;
		unsigned int channelPositionMS = 0;
		unsigned int soundLengthMS = 0;
		float channelVolume = 0.0f;
		float pen = 0.0f;

		_result = selectedSound->_sound->getName(fileName, FILENAME_LENGTH);
		assert(!_result);

		_result = selectedSound->_sound->getFormat(&type, &format, NULL, NULL);
		assert(!_result);

		_result = selectedSound->_channel->getFrequency(&frequency);
		assert(!_result);

		_result = selectedSound->_sound->getLength(&soundLengthMS, FMOD_TIMEUNIT_MS);
		assert(!_result);

		long lenMin = soundLengthMS / 60000;
		soundLengthMS = soundLengthMS - 60000 * lenMin;

		long lenSec = soundLengthMS / 1000;
		soundLengthMS = soundLengthMS - 1000 * lenSec;

		_result = selectedSound->_channel->getPosition(&channelPositionMS, FMOD_TIMEUNIT_MS);
		assert(!_result);

		long posMin = channelPositionMS / 60000;
		channelPositionMS = channelPositionMS - 60000 * posMin;

		long posSec = channelPositionMS / 1000;
		channelPositionMS = channelPositionMS - 1000 * posSec;

		_result = selectedSound->_channel->getVolume(&channelVolume);
		assert(!_result);

		//_console.print("Name: %s", fileName);
		////print format
		////print type
		//_console.print("Frequency: %f", frequency);

		_console.refresh();
		_console.print("Name: %s", fileName);
		_console.print("Format: %s", mapFormat[format].c_str());
		_console.print("Type: %s", mapType[type].c_str());
		_console.print("Frequency: %f", frequency);
		_console.print("Position: %ld:%ld of %ld:%ld", posMin, posSec, lenMin, lenSec);
		_console.print("Volume: %f", channelVolume);
		_console.print("Balance: %f", selectedSound->channelPan);
	}
}

void cSoundManager::increaseVolume()
{
	if (selectedSound)
	{
		float volume = 0.0f;
		_result = selectedSound->_channel->getVolume(&volume);
		assert(!_result);
		volume = (volume >= 1.0f) ? 1.0f : (volume + 0.1f);
		_result = selectedSound->_channel->setVolume(volume);
		assert(!_result);
	}
}
void cSoundManager::decreaseVolume()
{
	if (selectedSound)
	{
		float volume = 0.0f;
		_result = selectedSound->_channel->getVolume(&volume);
		assert(!_result);
		volume = (volume <= 0.0f) ? 0.0f : (volume - 0.1f);
		_result = selectedSound->_channel->setVolume(volume);
		assert(!_result);
	}
}
void cSoundManager::increasePan()
{
	if (selectedSound)
	{
		selectedSound->channelPan = selectedSound->channelPan >= 1.0f ? 1.0f : selectedSound->channelPan + 0.1f;
		_result = selectedSound->_channel->setPan(selectedSound->channelPan);
		assert(!_result);
	}
}
void cSoundManager::decreasePan()
{
	if (selectedSound)
	{
		selectedSound->channelPan = selectedSound->channelPan <= -1.0f ? -1.0f : selectedSound->channelPan - 0.1f;
		_result = selectedSound->_channel->setPan(selectedSound->channelPan);
		assert(!_result);
	}
}
void cSoundManager::increasePitch()
{
	if (selectedSound)
	{
		float pitch = 1.0f;
		_result = selectedSound->_channel->getPitch(&pitch);
		assert(!_result);
		pitch = (pitch >= 2.0f) ? 2.0f : (pitch + 0.1f);
		_result = selectedSound->_channel->setPitch(pitch);
		assert(!_result);
	}
}
void cSoundManager::decreasePitch()
{
	if (selectedSound)
	{
		float pitch = 1.0f;
		_result = selectedSound->_channel->getPitch(&pitch);
		assert(!_result);
		pitch = (pitch <= 0.1f) ? 0.1f : (pitch - 0.1f);
		_result = selectedSound->_channel->setPitch(pitch);
		assert(!_result);
	}
}
void cSoundManager::increasePlabackSpeed()
{
	if (selectedSound)
	{
		float frequency = 0.0f;
		_result = selectedSound->_channel->getFrequency(&frequency);
		assert(!_result);
		frequency += 500.0f;
		_result = selectedSound->_channel->setFrequency(frequency);
		assert(!_result);
	}
}
void cSoundManager::decreasePlaybackSpeed()
{
	if (selectedSound)
	{
		float frequency = 0.0f;
		_result = selectedSound->_channel->getFrequency(&frequency);
		assert(!_result);
		frequency -= 500.0f;
		_result = selectedSound->_channel->setFrequency(frequency);
		assert(!_result);
	}
}

