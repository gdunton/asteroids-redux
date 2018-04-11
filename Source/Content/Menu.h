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

class Model2D;
class GameState;

class Menu
{
private:
	GameState* parentState;

	Font font;

	std::shared_ptr<std::vector<MenuItem>> menuItems;
	// The index of currently selected menuItem
	int currentItemIndex;

	// The model for the pointer that indicates the selected item 
	// and the y offset from center
	Model2D* pointerModel;
	int pointerOffset;

	KeyboardState prevKbState;	

public:
	Menu();
	Menu(GameState* parent, std::shared_ptr<std::vector<MenuItem>> items, int pointerOffset);
	~Menu();

	// Called each frame to check input
	void Update();

	// Called each frame to draw to screen
	void Render();
};

#endif