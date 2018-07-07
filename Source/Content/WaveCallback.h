//-----------------------------------------------------------
// File: WaveCallback.h
// Description: inherits interface IXAudio2VoiceCallback. 
//				Impliments only OnStreamEnd to set the stopped
//				flag in a parent channel
//-----------------------------------------------------------

#ifndef WAVECALLBACK_H
#define WAVECALLBACK_H

#include <xaudio2.h>

class Channel;

class WaveCallback : public IXAudio2VoiceCallback
{
public:
	WaveCallback() noexcept : parent(nullptr) {}
	virtual ~WaveCallback() = default;

	WaveCallback(const WaveCallback& other) = delete;
	WaveCallback(WaveCallback&& other) noexcept = default;
	WaveCallback& operator=(const WaveCallback& other) = delete;
	WaveCallback& operator=(WaveCallback&& other) noexcept = default;

	void Init(Channel* _parent)
	{
		parent = _parent;
	}

	// Called when the voice has just finished playing a contiguous audio stream
	void __stdcall OnStreamEnd() override;

	// Unused method stubs
	void __stdcall OnVoiceProcessingPassEnd() override {}
	void __stdcall OnVoiceProcessingPassStart(UINT32 /*SamplesRequired*/) override {}
	void __stdcall OnBufferEnd(void* /*bufferContext*/) override {}
	void __stdcall OnBufferStart(void* /*bufferContext*/) override {}
	void __stdcall OnLoopEnd(void* /*bufferContext*/) override {}
	void __stdcall OnVoiceError(void* /*bufferContext*/, HRESULT /*error*/) override {}

private:
	Channel * parent;
};

#endif
