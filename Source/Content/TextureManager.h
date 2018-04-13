//-----------------------------------------------------------
// File: TextureManager.h
// Description: Singleton that stores all textures. Creates
//				a single transparent texture used for backgrounds
//-----------------------------------------------------------

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

typedef ID3D11ShaderResourceView Texture;

class TextureManager : public Singleton<TextureManager>
{
public:
	friend Singleton<TextureManager>;

public:
	Texture* GetTexture(const std::string& name);
	Vector2 GetDimensions(const std::string& name);

protected:
	TextureManager();
	~TextureManager();

private:
	// Creates a texture that is 
	void CreateTransparencyRect();

	struct ResourcePair {
		ID3D11ShaderResourceView* view;
		ID3D11Texture2D* texture;
	};

	std::map<std::string, ResourcePair> m_textureMap;
	std::string m_assetsDir;
};


#endif