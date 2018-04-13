//-----------------------------------------------------------
// File: MenuItem.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "MenuItem.h"
#include <utility>

MenuItem::MenuItem(std::function<void()> selectAction, Vector2 pos, String text, Color color) : 
	centerPos(pos), 
	text(std::move(text)), 
	selectAction(std::move(selectAction))
{
}

void MenuItem::Select() const
{
	selectAction();
}

void MenuItem::Render() const
{
	Vector2 size = font.GetTextSize(text);
	Vector2 position = centerPos - static_cast<Vector2>(size / 2);
	font.DrawString(text, position);
}

Vector2 MenuItem::GetPosition() const
{
	return centerPos;
}
