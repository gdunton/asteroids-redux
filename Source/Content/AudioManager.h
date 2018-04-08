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

class AudioManager : public Singleton< AudioManager >
{
public:
	friend Singleton< AudioManager >;

	// Functions protected for the singleton
protected:
	AudioManager();
	~AudioManager();

private:
	// Collection of all wav sound assets
	std::map< String, std::shared_ptr<MyWav> > soundMap;

	// List of all the channels
	std::vector< Channel > channels;

	IXAudio2* xAudio2;
	IXAudio2MasteringVoice* masteringVoice;

	String assetsDir;
	static const int NUM_CHANNELS;

public:
	// Public static functions for easier calling


	static void LoadAllAssets();

	// Attaches sound to channel. Returns IStoppable allowing the sound to
	// be stored and stopped by the caller
	static IStoppable* PlaySoundByName( String soundName );

	// Stops all channels
	static void StopAllSound();

private:
	void LoadWavFile( String filename, String assetName, bool soundLoops = false );

	// Internal functions with simpler access to member variables
	void InLoadAllAssets();
	IStoppable* InPlaySoundByName( String soundName );
	void InStopAllSound();
};

#endif