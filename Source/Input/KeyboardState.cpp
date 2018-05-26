//-----------------------------------------------------------
// File: KeyboardState.cpp
//-----------------------------------------------------------

#include "KeyboardState.h"
#include <string>
#include "Strings.h"

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

std::ostream& operator<<(std::ostream& str, const KeyboardState& state)
{
	for (unsigned int i = 0; i < 256; ++i)
	{
		const auto keyState = state.GetKeyState(i);
		if (keyState == KeyState::pressed)
		{
			str << i << ',';
		}
	}
	return str;
}

KeyboardState KeyboardState::FromString(const std::string& str)
{
	std::array<KeyState, 256> keyList;
	keyList.fill(KeyState::unpressed);

	for (const auto& key : SplitString(str, ','))
	{
		const int value = std::stoi(std::string(key));
		keyList.at(value) = KeyState::pressed;
	}

	return KeyboardState(keyList);
}