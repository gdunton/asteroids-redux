//-----------------------------------------------------------
// File: Channel.h
// Description: Channel inherits IStoppable to make it easy to
//				pass a pointer to a channel for stopping it. Uses
//				an WaveCallback to register when the sound has finished
//-----------------------------------------------------------

#ifndef CHANNEL_H
#define CHANNEL_H

#include <XAudio2.h>
#include "WaveCallback.h"

class MyWav;

class Channel : public IStoppable
{
private:
	IXAudio2SourceVoice* sourceVoice;

	bool stopped;
	WaveCallback callbacks;

public:
	Channel();
	~Channel();

	// Creates sourceVoice and supplies callbacks
	void Init( IXAudio2* xAudio2, WAVEFORMATEX* format );
	void Destroy();

	// submit sound effect to the buffer and start playback
	void PlayWav( MyWav* myWav );
	bool PlayingSound();

	void VStop();

	void SetStopped( bool value ) { stopped = value; }
};

#endif