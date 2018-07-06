//-----------------------------------------------------------
// File: Font.cp
//-----------------------------------------------------------

#include "Font.h"
#include "GraphicsDeviceManager.h"
#include <SpriteFont.h>
#include "../Utilities/Strings.h"
#include "../Content/Content.h"

DirectX::SpriteBatch* Font::spriteBatch = nullptr;

Font::Font(const GraphicsDeviceManager& graphicsManager, Color color) :
	color(color)
{
	// Create the font description
	std::string assetsDir = Content::GetAssetsDir();
	assetsDir += "arial.spritefont";

	std::wstring wStr = StringToWString(assetsDir);

	font = std::make_unique<DirectX::SpriteFont>(graphicsManager.GetDevice(), wStr.c_str());

	// Store the color 
	this->color = color;

	spriteBatch = graphicsManager.GetSpriteBatch();
}

void Font::DrawString(const std::string& str, const Vector2& position) const
{
	// Actually draw the text to screen
	spriteBatch->Begin();

	std::wstring wStr = StringToWString(str);
	font->DrawString(spriteBatch, wStr.c_str(), position);

	spriteBatch->End();
}

Vector2 Font::GetTextSize(const std::string& str) const
{
	std::wstring wStr = StringToWString(str);
	DirectX::XMVECTOR result = font->MeasureString(wStr.c_str());

	return Vector2(DirectX::XMVectorGetX(result), DirectX::XMVectorGetY(result));
}
