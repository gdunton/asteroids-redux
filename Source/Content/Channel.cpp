//-----------------------------------------------------------
// File: Channel.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "Channel.h"

#include "MyWav.h"

Channel::Channel()
{
	sourceVoice = 0;
	stopped = true;
}

Channel::~Channel()
{
	if( sourceVoice )
	{
		ASSERTMSG( false, "Shouldn't be here. Remember to destroy Channel" );
	}
}

void Channel::Init( IXAudio2* xAudio2, WAVEFORMATEX* format )
{
	if( sourceVoice )
	{
		// source voice has already been initialized
		sourceVoice->DestroyVoice();
		sourceVoice = 0;
	}
	callbacks.Init( this );

	// Initialize the voice with the callbacks
	xAudio2->CreateSourceVoice( &sourceVoice, format, 0,
		XAUDIO2_DEFAULT_FREQ_RATIO, &callbacks, 0, 0 );
}

void Channel::Destroy()
{
	sourceVoice->DestroyVoice();
	sourceVoice = 0;
}

bool Channel::PlayingSound()
{
	return !stopped;
}

void Channel::PlayWav( MyWav* wav )
{
	sourceVoice->SubmitSourceBuffer( wav->GetBuffer() );
	sourceVoice->Start( 0 );
	stopped = false;
}

void Channel::VStop()
{
	sourceVoice->Stop();
	sourceVoice->FlushSourceBuffers();
	stopped = true;
}