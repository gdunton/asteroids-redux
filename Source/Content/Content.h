#pragma once

#include "../Graphics/GraphicsDeviceManager.h"
#include "TextureManager.h"
#include "AudioManager.h"
#include "ModelManager.h"

class Content
{
public:
	explicit Content(GraphicsDeviceManager& graphics);

	const ModelManager& Models() const;
	const TextureManager& Textures() const;
	AudioManager& Audio();
	const GraphicsDeviceManager& Graphics() const;
private:
	ModelManager models;
	TextureManager textures;
	AudioManager audio;

	GraphicsDeviceManager& graphics;
};
