//-----------------------------------------------------------
// File: TextureManager.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "TextureManager.h"
#include "..\Graphics\GraphicsDeviceManager.h"
#include <WICTextureLoader.h>

template<> TextureManager* Singleton<TextureManager>::instance = NULL;

TextureManager::TextureManager()
{
	// Get the asset path
	GetAssetsDir( m_assetsDir );

	CreateTransparencyRect();
}

TextureManager::~TextureManager()
{
	for( auto begin = m_textureMap.begin(), end = m_textureMap.end(); begin != end;
		++begin )
	{
		if( begin->second ) 
		{
			begin->second->Release();
			begin->second = NULL;
		}
	}
}

Texture* TextureManager::GetTexture( const String& name )
{
	// Find the texture in the map 
	return m_textureMap[name];
}

void TextureManager::CreateTransparencyRect()
{
	// Create the texture
	//IDirect3DTexture9* tex = NULL;
	//D3DXCreateTexture( GraphicsDeviceManager::GetInstance().GetDevice(), 1, 1, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tex );


	DirectX::CreateWICTextureFromMemory(GraphicsDeviceManager::GetInstance().GetDevice(), )

	//ASSERT( tex != NULL );

	//// load the pixel into it
	//D3DLOCKED_RECT lockRect;
	//HRESULT hr = tex->LockRect( 0, &lockRect, NULL, 0 );
	//ASSERT( SUCCEEDED( hr ) );
	//BYTE* bits = (BYTE*)lockRect.pBits;
	//Color color = D3DCOLOR_ARGB( 255, 0, 0, 0 ); // Set the color to black
	//
	//for( int i = 0; i < 1; ++i )
	//{
	//	for( int j = 0; j < 1; ++j )
	//	{
	//		memcpy( &bits[ lockRect.Pitch * i + 4 * j ], &color, 4 );
	//	}
	//}

	//hr = tex->UnlockRect( 0 );
	//ASSERT( SUCCEEDED( hr ) );
	//
	//// save in the map as transparency
	//m_textureMap["transparent"] = tex;
}