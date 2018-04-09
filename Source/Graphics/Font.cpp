//-----------------------------------------------------------
// File: Font.cp
//-----------------------------------------------------------

#include "STD.h"
#include "Font.h"
#include "GraphicsDeviceManager.h"
#include <SpriteFont.h>

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

void Font::Initialize(int height, bool italic, Color _color)
{
	// Create the font description
	String assetsDir;
	GetAssetsDir(assetsDir);
	assetsDir += "arial.spritefont";

	WString wStr;
	StringToWString(assetsDir, wStr);

	font = new DirectX::SpriteFont(GraphicsDeviceManager::GetInstance().GetDevice(), wStr.c_str());

	// Store the color 
	color = _color;
}

void Font::Destroy()
{
}

void Font::DrawString( String str, MyVector2 position )
{
	// Actually draw the text to screen
	GraphicsDeviceManager::GetInstance().GetSpriteBatch()->Begin();
	
	WString wStr;
	StringToWString(str, wStr);
	font->DrawString(GraphicsDeviceManager::GetInstance().GetSpriteBatch(), wStr.c_str(), position);

	GraphicsDeviceManager::GetInstance().GetSpriteBatch()->End();
}

MyVector2 Font::GetTextSize( String str )
{
	WString wStr;
	StringToWString(str, wStr);
	DirectX::XMVECTOR result = font->MeasureString(wStr.c_str());

	return MyVector2(DirectX::XMVectorGetX(result), DirectX::XMVectorGetY(result));
}