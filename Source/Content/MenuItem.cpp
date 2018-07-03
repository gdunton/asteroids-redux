//-----------------------------------------------------------
// File: MenuItem.cpp
//-----------------------------------------------------------

#include "MenuItem.h"
#include <utility>

MenuItem::MenuItem(const GraphicsDeviceManager& graphics, std::function<void()> selectAction, Vector2 pos, std::string text, Color /*color*/) : 
	centerPos(pos), 
	text(std::move(text)), 
	font(graphics),
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
