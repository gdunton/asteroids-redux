//-----------------------------------------------------------
// File: Menu.h
// Description: Menu is intended to collect MenuItems together
//				with the input code for navigation
//-----------------------------------------------------------

#ifndef MENU_H
#define MENU_H

#include "../Graphics/Font.h"
#include "MenuItem.h"
#include "../Input/KeyboardState.h"
#include "../Graphics/Model2D.h"

class Model2D;
class GameState;

class Menu
{
private:
	Font font;

	std::shared_ptr<std::vector<MenuItem>> menuItems;
	// The index of currently selected menuItem
	int currentItemIndex = 0;

	// The model for the pointer that indicates the selected item 
	// and the y offset from center
	Model2D pointerModel;
	int pointerOffset = 0;

	KeyboardState prevKbState;

public:
	Menu(GraphicsDeviceManager& graphics, GameState* parent, std::shared_ptr<std::vector<MenuItem>> items, int pointerOffset);

	// Called each frame to check input
	void Update(const KeyboardState& keyboardState);

	// Called each frame to draw to screen
	void Render() const;
};

#endif
