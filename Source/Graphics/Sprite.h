//-----------------------------------------------------------
// File: Sprite.h
// Description: Container for the DirectX sprite. allows drawing 
//				of textures onto screen. Textures are retrieved
//				from the texture manager by the sprite itself on
//				initialization
//-----------------------------------------------------------

#ifndef SPRITE_H
#define SPRITE_H
#include "../Content/TextureManager.h"
#include <SpriteBatch.h>

class Sprite
{
private:
	Texture* m_pTex;

	static DirectX::SpriteBatch* spriteBatch;

	MyVector2 spriteSize;

public:
	Sprite();

	void Initialize( const String& textureName );
	void Draw( const MyVector2& _position, const MyVector2& _size, const float alpha );

	static void SetSpriteBatch(DirectX::SpriteBatch* sb);
};

#endif