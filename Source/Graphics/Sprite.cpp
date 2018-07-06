//-----------------------------------------------------------
// File: Sprite.cpp
//-----------------------------------------------------------
#include "Sprite.h"
#include "GraphicsDeviceManager.h"
#include "../Debugging/Error.h"
#include <d3d11.h>
#include <SimpleMath.h>

DirectX::SpriteBatch* Sprite::spriteBatch = nullptr;

Sprite::Sprite(const TextureManager& textureManager, const std::string& textureName)
{
	// Get the texture from the texture manager. If a null
	// value is returned then stop the program.
	m_pTex = textureManager.GetTexture(textureName);
	ASSERT( m_pTex != NULL );

	spriteSize = textureManager.GetDimensions(textureName);
}

void Sprite::Draw(const Vector2& position, float alpha) const
{
	spriteBatch->Begin();
	spriteBatch->Draw(m_pTex, position, DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, alpha));
	spriteBatch->End();
}

void Sprite::SetSpriteBatch(DirectX::SpriteBatch* sb)
{
	spriteBatch = sb;
}
