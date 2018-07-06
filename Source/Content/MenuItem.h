//-----------------------------------------------------------
// File: MenuItem.h
// Description: Represents a single entity on a menu. Draws a 
//				string and can call a function when selected
//-----------------------------------------------------------

#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <functional>
#include "../Graphics/Font.h"

class GameState;

class MenuItem
{
public:
	MenuItem(const GraphicsDeviceManager& graphics, std::function<void()> selectAction,
	         Vector2 pos, std::string text, Color color);

	// Calls the delegate
	void Select() const;

	// Draw the string to screen
	void Render() const;

	Vector2 GetPosition() const;

private:
	Vector2 centerPos;
	std::string text;
	Font font;

	// Function called when the item has select called in it
	std::function<void()> selectAction;
};

#endif
