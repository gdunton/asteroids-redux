//-----------------------------------------------------------
// File: KeyboardState.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "KeyboardState.h"

KeyboardState::KeyboardState()
{
	for( unsigned short i = 0; i < 256; ++i )
	{
		keyList[i] = unpressed;
	}
}

KeyState KeyboardState::GetKeyState( KeyCode key )
{
	return keyList[key];
}

KeyboardState& KeyboardState::operator=( const KeyboardState& source )
{
	for( unsigned short i = 0; i < 256; ++i )
	{
		keyList[i] = source.keyList[i];
	}

	return *this;
}