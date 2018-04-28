//-----------------------------------------------------------
// File: WaveCallback.h
// Description: 
//-----------------------------------------------------------

#include "WaveCallback.h"

#include "Channel.h"

void WaveCallback::OnStreamEnd()
{
	// Sets the parents stopped flag
	parent->SetStopped( true );
} 