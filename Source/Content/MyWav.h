//-----------------------------------------------------------
// File: MyWav.h
// Description: Class wrapping WinWav to manage loading wav files,
//				sound buffer data and wave formats
//-----------------------------------------------------------

#ifndef MYWAV_H
#define MYWAV_H

#include <xaudio2.h>
#include "../Utilities/WinWav.h"

using Byte = unsigned char;

class MyWav
{
public:
	MyWav() = default;

	// Loads the wav file via a filename. "loopSound" flags whether the sound is
	// made for continious play
	bool LoadFile(const std::string& filename, bool loopSound = false );

	XAUDIO2_BUFFER* GetBuffer() { return &buffer; }
	WAVEFORMATEX* GetWavFormat();
	bool GetLooping() const { return looping; }

private:
	WinWav wav;

	// The actual sound data
	std::unique_ptr<Byte[]> wavData;
	XAUDIO2_BUFFER buffer = {};

	bool looping = false;
};

#endif