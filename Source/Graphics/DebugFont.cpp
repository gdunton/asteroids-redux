//-----------------------------------------------------------
// File: DebugFont.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "DebugFont.h"
#ifdef DEBUG

template<> DebugFont* Singleton<DebugFont>::instance = 0;

DebugFont::DebugFont()
{
	font.Initialize(20, false, "Arial", WHITE );
}

void DebugFont::DrawString( String text, MyVector2 position )
{
	DebugFont::GetInstance().font.DrawString( text, position );
}

DebugFont::~DebugFont()
{
	font.Destroy();
}
#endif