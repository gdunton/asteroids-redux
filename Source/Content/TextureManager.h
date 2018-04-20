//-----------------------------------------------------------
// File: TextureManager.h
// Description: Singleton that stores all textures. Creates
//				a single transparent texture used for backgrounds
//-----------------------------------------------------------

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

class GraphicsDeviceManager;
using Texture = ID3D11ShaderResourceView;

class TextureManager
{
public:
	TextureManager(GraphicsDeviceManager& graphicsManager);

	Texture* GetTexture(const std::string& name) const;
	Vector2 GetDimensions(const std::string& name) const;

private:
	// Creates a texture that is 
	void CreateTransparencyRect(GraphicsDeviceManager& graphicsManager);

	struct ResourcePair
	{
		ComPtr<ID3D11ShaderResourceView> view;
		ComPtr<ID3D11Texture2D> texture;
	};

	std::map<std::string, ResourcePair> m_textureMap;
	std::string m_assetsDir;
};


#endif
