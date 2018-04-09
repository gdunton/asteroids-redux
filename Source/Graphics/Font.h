//-----------------------------------------------------------
// File: Font.h
// Description: Container for the DirectX font. Allows the drawing
//				of text onto screen as well as finding the size of 
//				a string when it is rendered
//-----------------------------------------------------------

#ifndef FONT_H
#define FONT_H

#include <SpriteFont.h>

class Font
{
private:
	DirectX::SpriteFont* font;

	Color color;

public:
	Font();
	~Font();

	void Initialize(int height, bool italic, Color color);
	void Destroy();

	// Renders text to screen. Uses screen position NOT world pos
	void DrawString( String str, Vector2 position );

	// Returns the size of the text when rendered
	Vector2 GetTextSize( String str );
};

#endif