//-----------------------------------------------------------
// File: MenuItem.h
// Description: Represents a single entity on a menu. Draws a 
//				string and can call a function when selected
//-----------------------------------------------------------

#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include "..\Graphics\Font.h"
#include "..\Utilities\FastDelegate.h"

class GameState;

typedef fastdelegate::FastDelegate0<> MenuSelectDelegate;

class MenuItem
{
private:
	Vector2 centerPos;
	String text;
	Font font;

	// Function called when the item has select called in it
	MenuSelectDelegate selectAction;

public:

	MenuItem();
	MenuItem( GameState* parent, MenuSelectDelegate _selectAction, 
		Vector2 strinCenterPos, String _text, int height, String fontName, Color color);
	~MenuItem();

	// Calls the delegate
	void Select();

	// Draw the string to screen
	void Render();

	Vector2 GetPosition() { return centerPos; }
};

#endif