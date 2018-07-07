//-----------------------------------------------------------
// File: Keyboard.h
// Description: Singleton managing the input from the keyboard.
//				Contains a representation of each key which is 
//				toggled from pressed to unpressed and back by
//				callback functions put into the window message loop
//-----------------------------------------------------------

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <array>
#include "KeyboardState.h"

class Keyboard
{
public:
	Keyboard() noexcept;

	// Returns the state of a single key
	KeyState GetKeyState(KeyCode key);
	void SetKeyState(KeyCode key, KeyState state);

	// Outputs the state of the entire keyboard
	KeyboardState GetKeyboardState() const;

private:
	std::array<KeyState, 256> keys;
};

#endif
