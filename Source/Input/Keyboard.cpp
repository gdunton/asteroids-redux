//-----------------------------------------------------------
// File: Keyboard.cpp
//-----------------------------------------------------------

#include "Keyboard.h"

Keyboard::Keyboard() :
	keys()
{
	keys.fill(KeyState::unpressed);
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
KeyboardState Keyboard::GetKeyboardState() const
{
	return KeyboardState(keys);
}
