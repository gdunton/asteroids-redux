//-----------------------------------------------------------
// File: Keyboard.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "Keyboard.h"

// Callback functions to communicate key messages from the window
// to the keyboard singleton
void SetKeyStatePressed( WPARAM wParam, LPARAM lParam )
{
	Keyboard& keyboard = Keyboard::GetInstance();

	keyboard.SetKeyState( wParam, pressed );
}

void SetKeyStateUnpressed( WPARAM wParam, LPARAM lParam )
{
	Keyboard& keyboard = Keyboard::GetInstance();

	keyboard.SetKeyState( wParam, unpressed );
}

template<> Keyboard* Singleton<Keyboard>::instance = 0;

Keyboard::Keyboard()
{
	// Initialize all keys
	for( unsigned short i = 0; i < 256; ++i )
	{
		keys[i] = unpressed;
	}
}

KeyState Keyboard::GetKeyState( KeyCode key )
{
	return keys[key];
}

void Keyboard::SetKeyState( KeyCode key, KeyState state )
{
	keys[key] = state;
}

// Outputs the state of the entire keyboard
void Keyboard::GetKeyboardState( KeyboardState& outState )
{
	for( unsigned short i = 0; i < 256; ++i )
	{
		outState.keyList[i] = keys[i];
	}
}