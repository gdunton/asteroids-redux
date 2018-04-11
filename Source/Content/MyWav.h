//-----------------------------------------------------------
// File: MyWav.h
// Description: Class wrapping WinWav to manage loading wav files,
//				sound buffer data and wave formats
//-----------------------------------------------------------

#ifndef MYWAV_H
#define MYWAV_H

#include <XAudio2.h>
#include "../Utilities/WinWav.h"

typedef unsigned char Byte;

class MyWav
{
private:
	WinWav wav;

	// The actual sound data
	Byte* wavData;
	XAUDIO2_BUFFER buffer;

	bool looping;

public:
	MyWav();
	~MyWav();

	// Denied operations
	MyWav( const MyWav& wav );
	MyWav& operator=( const MyWav& wav );

	// Loads the wav file via a filename. "loopSound" flags whether the sound is
	// made for continious play
	bool LoadFile( String filename, IXAudio2* xAudio2, bool loopSound = false );

	XAUDIO2_BUFFER* GetBuffer() { return &buffer; }
	WAVEFORMATEX* GetWavFormat();
	bool GetLooping() { return looping; }
};

#endif