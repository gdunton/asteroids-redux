//-----------------------------------------------------------
// File: Font.cp
//-----------------------------------------------------------

#include "STD.h"
#include "Font.h"
#include "GraphicsDeviceManager.h"

Font::Font()
{
	font = NULL;
	color = BLACK;
}

Font::~Font()
{
	if( font )
	{
 		//font->OnLostDevice();
		font = NULL;
	}
}

void Font::Initialize( int height, bool italic, String name, Color _color )
{
	// Create the font description
	//D3DXFONT_DESC fd;
	//ZeroMemory( &fd, sizeof( fd ) );
	//fd.Height = height;
	//fd.Width = 0;
	//fd.Weight = FW_NORMAL;
	//fd.MipLevels = 1;
	//fd.Italic = italic;
	//fd.CharSet = DEFAULT_CHARSET;
	//fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	//fd.Quality = ANTIALIASED_QUALITY;
	//fd.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	//// Convert the string into a wide string then input into description
	//WString wStr;
	//StringToWString( name, wStr );
	//wcscpy( fd.FaceName, wStr.c_str() );
	//// Create the font
	//D3DXCreateFontIndirect( GraphicsDeviceManager::GetInstance().GetDevice(),
	//	&fd, &font );

	//// Store the color 
	//color = _color;
}

void Font::Destroy()
{
}

void Font::DrawString( String str, MyVector2 position )
{
	RECT fontRect = { position.x, position.y, 0, 0 };

	// Actually draw the text to screen
	//font->DrawTextA( NULL, str.c_str(), -1, &fontRect, DT_LEFT | DT_NOCLIP, color );
}

MyVector2 Font::GetTextSize( String str )
{
	RECT fontRect = { 0, 0, 0, 0 };

	// Get the size of the string from the font
	/*int textHeight = font->DrawTextA( NULL, str.c_str(), -1, &fontRect, 
		DT_CALCRECT | DT_NOCLIP | DT_LEFT, BLACK );
	fontRect.bottom = fontRect.top + textHeight;*/

	return MyVector2( fontRect.right, fontRect.bottom );
}