//-----------------------------------------------------------
// File: WaveCallback.h
// Description: 
//-----------------------------------------------------------

#include "STD.h"
#include "WaveCallback.h"

#include "Channel.h"

void WaveCallback::OnStreamEnd()
{
	// Sets the parents stopped flag
	parent->SetStopped( true );
} 