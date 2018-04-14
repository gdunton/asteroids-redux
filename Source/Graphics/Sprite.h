//-----------------------------------------------------------
// File: Sprite.h
// Description: Container for the DirectX sprite. allows drawing 
//				of textures onto screen. Textures are retrieved
//				from the texture manager by the sprite itself on
//				initialization
//-----------------------------------------------------------

#ifndef SPRITE_H
#define SPRITE_H

#include <SpriteBatch.h>
#include "../Content/TextureManager.h"

class Sprite
{
private:
	Texture* m_pTex;

	static DirectX::SpriteBatch* spriteBatch;

	Vector2 spriteSize;

public:
	Sprite();

	void Initialize(TextureManager& textureManager, const std::string& textureName);
	void Draw( const Vector2& _position, const Vector2& _size, const float alpha );

	static void SetSpriteBatch(DirectX::SpriteBatch* sb);
};

#endif