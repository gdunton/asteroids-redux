//-----------------------------------------------------------
// File: Sprite.cpp
//-----------------------------------------------------------
#include "STD.h"
#include "Sprite.h"
#include "GraphicsDeviceManager.h"

DirectX::SpriteBatch* Sprite::spriteBatch = nullptr;

Sprite::Sprite()
{
	m_pTex = NULL;
}

void Sprite::Initialize( const String& textureName )
{
	// Get the texture from the texture manager. If a null
	// value is returned then stop the program.
	m_pTex = TextureManager::GetInstance().GetTexture(textureName);
	ASSERT( m_pTex != NULL );

	spriteSize = TextureManager::GetInstance().GetDimensions(textureName);
}

void Sprite::Draw( const MyVector2& _pos, const MyVector2& _size, const float alpha )
{
	spriteBatch->Begin();
	spriteBatch->Draw(m_pTex, _pos, DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, alpha));
	spriteBatch->End();
}

void Sprite::SetSpriteBatch(DirectX::SpriteBatch* sb) 
{
	spriteBatch = sb;
}
