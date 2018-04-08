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

private:
	std::map< String, Texture* > m_textureMap;
	String m_assetsDir;

public:
	Texture* GetTexture( const String& name );

protected:
	TextureManager();
	~TextureManager();

private:
	// Creates a texture that is 
	void CreateTransparencyRect();


};


#endif