//-----------------------------------------------------------
// File: AudioManager.h
// Description: AudioManager is a singleton that loads sound assets
//				and when play requests are sent, attaches the sounds
//				to a free channel
//-----------------------------------------------------------

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <XAudio2.h>
#include "MyWav.h"
#include "Channel.h"
#include <map>
#include <memory>
#include <vector>

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

private:
	// Collection of all wav sound assets
	std::map<std::string, std::shared_ptr<MyWav>> soundMap;

	// List of all the channels
	std::vector<Channel> channels;

	IXAudio2* xAudio2;
	IXAudio2MasteringVoice* masteringVoice;

	std::string assetsDir;
	static const int NUM_CHANNELS;

public:
	// Attaches sound to channel. Returns IStoppable allowing the sound to
	// be stored and stopped by the caller
	Channel* PlaySoundByName(const std::string& soundName);

	// Stops all channels
	void StopAllSound();

private:
	void LoadWavFile(const std::string& filename, const std::string& assetName, bool soundLoops = false);

	// Internal functions with simpler access to member variables
	void LoadAllAssets();
};

#endif
