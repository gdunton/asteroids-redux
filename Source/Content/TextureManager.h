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
	Texture* GetTexture( const String& name );
	MyVector2 GetDimensions( const String& name );

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

	std::map<String, ResourcePair> m_textureMap;
	String m_assetsDir;
};


#endif