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
#include "../Utilities/Vector2.h"

class Sprite
{
public:
	Sprite() = default;
	Sprite(const TextureManager& textureManager, const std::string& textureName);
	void Draw(const Vector2& position, float alpha) const;

	static void SetSpriteBatch(DirectX::SpriteBatch* sb);

private:
	Vector2 spriteSize;
	Texture* m_pTex = nullptr;

	static DirectX::SpriteBatch* spriteBatch;
};

#endif
