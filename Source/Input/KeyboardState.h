//-----------------------------------------------------------
// File: KeyboardState.h
// Description: Containes the states of all the keys at the point
//				that the keyboard state was requested from the 
//				keyboard
//-----------------------------------------------------------

#ifndef KEYBOARDSTATE_H
#define KEYBOARDSTATE_H

class Keyboard;

// Keycodes are the codes used by windows i.e. "A" or VK_ESCAPE
typedef unsigned int KeyCode;
enum KeyState { unpressed = 0, pressed = 1 };

class KeyboardState
{
public:
	friend class Keyboard;

	KeyboardState();

	KeyState GetKeyState( KeyCode key );

	KeyboardState& operator=( const KeyboardState& source );

private:
	KeyState keyList[256];
};

#endif