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

class Sprite
{
private:
	Texture* m_pTex;
	//LPD3DXSPRITE m_pSprite;

	MyVector2 spriteSize;

public:
	Sprite();
	~Sprite();

	void Initialize( const String& textureName );
	void Draw( const MyVector2& _position );
	void Draw( const MyVector2& _position, const MyVector2& _size );
	void Draw( const MyVector2& _position, const MyVector2& _size, const float alpha );
};

#endif