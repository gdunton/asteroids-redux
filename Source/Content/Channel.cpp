//-----------------------------------------------------------
// File: Channel.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "Channel.h"

#include "MyWav.h"

namespace {
	void DestroySourceVoice(IXAudio2SourceVoice* ptr)
	{
		if (ptr)
		{
			ptr->DestroyVoice();
		}
	}
}

Channel::Channel() :
	sourceVoice(nullptr, &DestroySourceVoice)
{
	stopped = true;
}

void Channel::Init(IXAudio2* xAudio2, WAVEFORMATEX* format)
{
	callbacks.Init(this);

	// Initialize the voice with the callbacks
	IXAudio2SourceVoice* newSourcePtr = nullptr;
	xAudio2->CreateSourceVoice(&newSourcePtr, format, 0,
	                           XAUDIO2_DEFAULT_FREQ_RATIO, &callbacks, nullptr, nullptr);

	sourceVoice = SourceVoicePtr(newSourcePtr, &DestroySourceVoice);
}

bool Channel::PlayingSound() const
{
	return !stopped;
}

void Channel::PlayWav(MyWav* wav)
{
	sourceVoice->SubmitSourceBuffer(wav->GetBuffer());
	sourceVoice->Start(0);
	stopped = false;
}

void Channel::Stop()
{
	sourceVoice->Stop();
	sourceVoice->FlushSourceBuffers();
	stopped = true;
}
