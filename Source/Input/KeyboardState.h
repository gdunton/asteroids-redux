//-----------------------------------------------------------
// File: KeyboardState.h
// Description: Containes the states of all the keys at the point
//				that the keyboard state was requested from the 
//				keyboard
//-----------------------------------------------------------

#ifndef KEYBOARDSTATE_H
#define KEYBOARDSTATE_H

#include <array>

// Keycodes are the codes used by windows i.e. "A" or VK_ESCAPE
using KeyCode = unsigned int;

enum class KeyState { unpressed = 0, pressed = 1 };

class KeyboardState
{
public:
	KeyboardState() noexcept;
	explicit KeyboardState(const std::array<KeyState, 256>& keyList) noexcept;

	KeyState GetKeyState(KeyCode key) const;

	static KeyboardState FromString(const std::string& str);

private:
	std::array<KeyState, 256> keyList;
};

std::ostream& operator<<(std::ostream& str, const KeyboardState& state);


#endif
