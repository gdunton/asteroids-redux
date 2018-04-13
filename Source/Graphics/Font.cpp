//-----------------------------------------------------------
// File: Font.cp
//-----------------------------------------------------------

#include "STD.h"
#include "Font.h"
#include "GraphicsDeviceManager.h"
#include <SpriteFont.h>

Font::Font(Color color) :
	color(color)
{
	// Create the font description
	String assetsDir;
	GetAssetsDir(assetsDir);
	assetsDir += "arial.spritefont";

	WString wStr;
	StringToWString(assetsDir, wStr);

	font = std::make_unique<DirectX::SpriteFont>(GraphicsDeviceManager::GetInstance().GetDevice(), wStr.c_str());

	// Store the color 
	this->color = color;
}

void Font::DrawString(const String& str, const Vector2& position) const
{
	// Actually draw the text to screen
	GraphicsDeviceManager::GetInstance().GetSpriteBatch()->Begin();

	WString wStr;
	StringToWString(str, wStr);
	font->DrawString(GraphicsDeviceManager::GetInstance().GetSpriteBatch(), wStr.c_str(), position);

	GraphicsDeviceManager::GetInstance().GetSpriteBatch()->End();
}

Vector2 Font::GetTextSize(const String& str) const
{
	WString wStr;
	StringToWString(str, wStr);
	DirectX::XMVECTOR result = font->MeasureString(wStr.c_str());

	return Vector2(DirectX::XMVectorGetX(result), DirectX::XMVectorGetY(result));
}
