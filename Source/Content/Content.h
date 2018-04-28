#pragma once

#include "../Graphics/GraphicsDeviceManager.h"
#include "TextureManager.h"
#include "AudioManager.h"
#include "ModelManager.h"
#include "../Graphics/Font.h"

class Content
{
public:
	explicit Content(GraphicsDeviceManager& graphics);

	const ModelManager& Models() const;
	const TextureManager& Textures() const;
	AudioManager& Audio();
	const GraphicsDeviceManager& Graphics() const;
	Font* DefaultFont();

	static std::string GetAssetsDir();
private:
	ModelManager models;
	TextureManager textures;
	AudioManager audio;

	Font defaultFont;

	GraphicsDeviceManager& graphics;
};
