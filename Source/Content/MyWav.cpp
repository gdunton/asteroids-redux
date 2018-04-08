//-----------------------------------------------------------
// File: MyWav.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "MyWav.h"

MyWav::MyWav()
{
	wavData = 0;
	looping = false;
}

MyWav::~MyWav()
{
	if( wavData )
	{
		delete[] wavData;
		wavData = 0;
	}
}

// Should never run copy constructor or equals op
MyWav::MyWav( const MyWav& wav )
{
	ASSERT( false );
}

MyWav& MyWav::operator=( const MyWav& wav )
{
	ASSERT( false );
	return *this;
}

bool MyWav::LoadFile( String filename, IXAudio2* xAudio2, bool loopSound )
{
	looping = loopSound;

	// convert string to wstring
	WString wString;
	StringToWString( filename, wString );

	wav.Open( &wString[0], 0, WAVEFILE_READ );

	// Get the format and size of the buffer
	WAVEFORMATEX* format = wav.GetFormat();
	DWORD size = wav.GetSize();

	// Create the array to store the buffer
	wavData = new Byte[ size ];

	// Read from the file into the buffer
	wav.Read( wavData, size, &size );

	// Sbumit the wav sample data using a buffer
	buffer.LoopBegin = 0;
	buffer.LoopLength = 0;
	buffer.pContext = 0;
	buffer.PlayBegin = 0;
	buffer.PlayLength = 0;

	buffer.pAudioData = wavData;
	buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
	buffer.AudioBytes = size;
	buffer.LoopCount = (loopSound ? XAUDIO2_LOOP_INFINITE : 0);	

	return true;
}

WAVEFORMATEX* MyWav::GetWavFormat()
{
	return wav.GetFormat();
}