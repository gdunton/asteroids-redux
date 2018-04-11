//-----------------------------------------------------------
// File: TextureManager.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "TextureManager.h"
#include "../Graphics/GraphicsDeviceManager.h"
#include "../GameMain/Globals.h"

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
		if (begin->second.texture != nullptr)
		{
			begin->second.texture->Release();
			begin->second.texture = nullptr;
		}

		if (begin->second.view != nullptr)
		{
			begin->second.view->Release();
			begin->second.view = nullptr;
		}
	}
}

Texture* TextureManager::GetTexture( const String& name )
{
	// Find the texture in the map 
	return m_textureMap[name].view;
}

Vector2 TextureManager::GetDimensions(const String& name) 
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	m_textureMap[name].texture->GetDesc(&desc);

	return Vector2(static_cast<float>(desc.Width), static_cast<float>(desc.Height));
}

void TextureManager::CreateTransparencyRect()
{
	// Create the texture
	D3D11_TEXTURE2D_DESC desc;

	desc.Width = static_cast<unsigned int>(WINDOW_WIDTH);
	desc.Height = static_cast<unsigned int>(WINDOW_HEIGHT);
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	ID3D11Texture2D* texture = NULL;

	float* buffer = new float[static_cast<int>(WINDOW_WIDTH) * static_cast<int>(WINDOW_HEIGHT) * 4];

	for (int y = 0; y < WINDOW_HEIGHT * WINDOW_WIDTH * 4; ++y) 
	{
		buffer[y] = 1.0f;
	}

	D3D11_SUBRESOURCE_DATA texData;
	texData.SysMemPitch = static_cast<int>(WINDOW_WIDTH) * sizeof(float) * 4;
	texData.SysMemSlicePitch = static_cast<int>(WINDOW_WIDTH) * static_cast<int>(WINDOW_HEIGHT) * sizeof(float) * 4;
	texData.pSysMem = (void*)buffer;

	HRESULT hr = GraphicsDeviceManager::GetInstance().GetDevice()->CreateTexture2D(&desc, &texData, &texture);
	ASSERT(hr == S_OK);

	delete[] buffer;

	ID3D11ShaderResourceView* resourceView = NULL;
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Texture2D.MostDetailedMip = 0;

	hr = GraphicsDeviceManager::GetInstance().GetDevice()->CreateShaderResourceView(texture, &viewDesc, &resourceView);
	ASSERT(hr == S_OK);

	ResourcePair p;
	p.texture = texture;
	p.view = resourceView;
	m_textureMap["transparent"] = p;
}
