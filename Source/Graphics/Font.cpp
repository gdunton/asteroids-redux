//-----------------------------------------------------------
// File: Font.cp
//-----------------------------------------------------------

#include "STD.h"
#include "Font.h"
#include "GraphicsDeviceManager.h"
#include <SpriteFont.h>

DirectX::SpriteBatch* Font::spriteBatch = nullptr;

Font::Font(GraphicsDeviceManager& graphicsManager, Color color) :
	color(color)
{
	// Create the font description
	std::string assetsDir;
	GetAssetsDir(assetsDir);
	assetsDir += "arial.spritefont";

	std::wstring wStr;
	StringToWString(assetsDir, wStr);

	font = std::make_unique<DirectX::SpriteFont>(graphicsManager.GetDevice(), wStr.c_str());

	// Store the color 
	this->color = color;

	spriteBatch = graphicsManager.GetSpriteBatch();
}

void Font::DrawString(const std::string& str, const Vector2& position) const
{
	// Actually draw the text to screen
	spriteBatch->Begin();

	std::wstring wStr;
	StringToWString(str, wStr);
	font->DrawString(spriteBatch, wStr.c_str(), position);

	spriteBatch->End();
}

Vector2 Font::GetTextSize(const std::string& str) const
{
	std::wstring wStr;
	StringToWString(str, wStr);
	DirectX::XMVECTOR result = font->MeasureString(wStr.c_str());

	return Vector2(DirectX::XMVectorGetX(result), DirectX::XMVectorGetY(result));
}
