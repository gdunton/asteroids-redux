//-----------------------------------------------------------
// File: KeyboardState.cpp
//-----------------------------------------------------------

#include "KeyboardState.h"

KeyboardState::KeyboardState() :
	keyList()
{
	keyList.fill(KeyState::unpressed);
}

KeyboardState::KeyboardState(const std::array<KeyState, 256>& keyList) :
	keyList(keyList)
{}

KeyState KeyboardState::GetKeyState(KeyCode key) const
{
	return keyList.at(key);
}
