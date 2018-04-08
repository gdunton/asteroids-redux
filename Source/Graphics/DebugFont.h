//-----------------------------------------------------------
// File: DebugFont.h
// Description: Singleton font for debugging to make it simpler
//				to print text onto the screen
//-----------------------------------------------------------

#ifndef DEBUG_FONT_H
#define DEBUG_FONT_H
#ifdef DEBUG
	
#include "Font.h"
class DebugFont : public Singleton<DebugFont>
{
public:
	friend Singleton<DebugFont>;
protected:
	DebugFont();

	// The font used
	Font font;

public:
	// Single function to draw text onto screen. Uses screen position
	// NOT world
	static void DrawString( String text, MyVector2 position );
	~DebugFont();
};

#endif
#endif