#include "Content.h"
#include "../Utilities/Strings.h"

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

std::string Content::GetAssetsDir()
{
	static std::string s_assetPath;

	if (s_assetPath.empty())
	{
		WCHAR dir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, dir);

		s_assetPath = WStringToString(dir);

		const auto found = s_assetPath.find_last_of("/\\");
		s_assetPath.erase(found, s_assetPath.length());
		s_assetPath += "\\Assets\\";
	}

	return s_assetPath;
}
