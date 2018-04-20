//-----------------------------------------------------------
// File: Font.h
// Description: Container for the DirectX font. Allows the drawing
//				of text onto screen as well as finding the size of 
//				a string when it is rendered
//-----------------------------------------------------------

#ifndef FONT_H
#define FONT_H

#include <memory>
#include <SpriteFont.h>

class GraphicsDeviceManager;

class Font
{
public:
	explicit Font(const GraphicsDeviceManager& graphicsManager, Color color = WHITE);

	// Renders text to screen. Uses screen position NOT world pos
	void DrawString(const std::string& str, const Vector2& position) const;

	// Returns the size of the text when rendered
	Vector2 GetTextSize(const std::string& str) const;

private:
	std::unique_ptr<DirectX::SpriteFont> font;
	Color color;

	static DirectX::SpriteBatch* spriteBatch;
};

#endif
