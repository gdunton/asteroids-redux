#include "STD.h"
#include "Content.h"

Content::Content(GraphicsDeviceManager& graphics) :
	textures(graphics),
	graphics(graphics),
	defaultFont(graphics)
{}

const ModelManager& Content::Models() const { return models; }
const TextureManager& Content::Textures() const { return textures; }
AudioManager& Content::Audio() { return audio; }
const GraphicsDeviceManager& Content::Graphics() const{ return graphics; }
Font* Content::DefaultFont() { return &defaultFont; }
