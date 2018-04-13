//-----------------------------------------------------------
// File: DebugFont.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "DebugFont.h"
#ifdef DEBUG

template<> DebugFont* Singleton<DebugFont>::instance = nullptr;

void DebugFont::DrawString( String text, Vector2 position )
{
	DebugFont::GetInstance().font.DrawString( text, position );
}

#endif