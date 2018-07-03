//-----------------------------------------------------------
// File: MyWav.cpp
//-----------------------------------------------------------

#include "MyWav.h"
#include "../Debugging/Error.h"
#include "../Utilities/Strings.h"

bool MyWav::LoadFile(const std::string& filename, bool loopSound )
{
	looping = loopSound;

	// convert string to wstring
	std::wstring wString = StringToWString( filename );

	wav.Open( &wString[0], nullptr, WAVEFILE_READ );

	// Get the size of the buffer
	DWORD size = wav.GetSize();

	// Create the array to store the buffer
	wavData = std::make_unique<Byte[]>(size);

	// Read from the file into the buffer
	wav.Read( wavData.get(), size, &size );

	// Sbumit the wav sample data using a buffer
	buffer.LoopBegin = 0;
	buffer.LoopLength = 0;
	buffer.pContext = nullptr;
	buffer.PlayBegin = 0;
	buffer.PlayLength = 0;

	buffer.pAudioData = wavData.get();
	buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
	buffer.AudioBytes = size;
	buffer.LoopCount = (loopSound ? XAUDIO2_LOOP_INFINITE : 0);	

	return true;
}

WAVEFORMATEX* MyWav::GetWavFormat()
{
	return wav.GetFormat();
}