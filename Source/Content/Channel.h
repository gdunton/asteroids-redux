//-----------------------------------------------------------
// File: Channel.h
// Description: Channel inherits IStoppable to make it easy to
//				pass a pointer to a channel for stopping it. Uses
//				an WaveCallback to register when the sound has finished
//-----------------------------------------------------------

#ifndef CHANNEL_H
#define CHANNEL_H

#include <XAudio2.h>
#include <memory>
#include "WaveCallback.h"

class MyWav;

using SourceVoicePtr = std::unique_ptr<IXAudio2SourceVoice, void(*)(IXAudio2SourceVoice*)>;

class Channel
{
public:
	Channel();

	// Creates sourceVoice and supplies callbacks
	void Init(IXAudio2* xAudio2, WAVEFORMATEX* format);

	// submit sound effect to the buffer and start playback
	void PlayWav(MyWav* myWav);
	bool PlayingSound() const;

	void Stop();

	void SetStopped(bool value) { stopped = value; }

private:
	SourceVoicePtr sourceVoice;

	bool stopped;
	WaveCallback callbacks;
};

#endif
