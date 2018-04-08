//-----------------------------------------------------------
// File: Keyboard.h
// Description: Singleton managing the input from the keyboard.
//				Contains a representation of each key which is 
//				toggled from pressed to unpressed and back by
//				callback functions put into the window message loop
//-----------------------------------------------------------

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "KeyboardState.h"

// Callback functions
void SetKeyStatePressed( WPARAM wParam, LPARAM lParam );
void SetKeyStateUnpressed( WPARAM wParam, LPARAM lParam );

class Keyboard : public Singleton< Keyboard >
{
public:
	// Allow the singleton to work
	friend Singleton<Keyboard>;
protected:

	Keyboard();

	KeyState keys[256];

public:
	// Returns the state of a single key
	KeyState GetKeyState( KeyCode key );

	void SetKeyState( KeyCode key, KeyState state );

	// Outputs the state of the entire keyboard
	void GetKeyboardState( KeyboardState& outState );

	
};

#endif