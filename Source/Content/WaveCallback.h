//-----------------------------------------------------------
// File: WaveCallback.h
// Description: inherits interface IXAudio2VoiceCallback. 
//				Impliments only OnStreamEnd to set the stopped
//				flag in a parent channel
//-----------------------------------------------------------

#ifndef WAVECALLBACK_H
#define WAVECALLBACK_H

#include <XAudio2.h>

class Channel;

class WaveCallback : public IXAudio2VoiceCallback
{
private:
	Channel* parent;

public:
	WaveCallback() : parent( nullptr ) {}
	~WaveCallback(){}

	void Init( Channel* _parent )
	{
		parent = _parent;
	}

	// Called when the voice has just finished playing a contiguous audio stream
	void __stdcall OnStreamEnd();

	// Unused method stubs
	void __stdcall OnVoiceProcessingPassEnd(){}
	void __stdcall OnVoiceProcessingPassStart( UINT32 SamplesRequired ) {}
	void __stdcall OnBufferEnd( void* bufferContext ) {}
	void __stdcall OnBufferStart( void* bufferContext ) {}
	void __stdcall OnLoopEnd( void* bufferContext ) {}
	void __stdcall OnVoiceError( void* bufferContext, HRESULT error ) {}
};

#endif