//-----------------------------------------------------------
// File: MenuItem.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "MenuItem.h"

MenuItem::MenuItem()
{
	text = "";
	centerPos = MyVector2(0,0);
}

MenuItem::MenuItem( GameState* parent, MenuSelectDelegate _selectAction, MyVector2 pos,
				   String _text, int height, String fontName, Color color) : 
		selectAction( _selectAction ), centerPos( pos ), text( _text )
{
	font.Initialize( height, false, color);
}

MenuItem::~MenuItem()
{
	font.Destroy();
}

void MenuItem::Select()
{
	selectAction();
}

void MenuItem::Render()
{
	MyVector2 size = font.GetTextSize( text );
	MyVector2 position = centerPos - static_cast<MyVector2>(size / 2);
	font.DrawString(text, position);
}