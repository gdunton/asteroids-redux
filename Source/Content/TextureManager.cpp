//-----------------------------------------------------------
// File: TextureManager.cpp
//-----------------------------------------------------------

#include "TextureManager.h"
#include "../Graphics/GraphicsDeviceManager.h"
#include "../GameMain/Globals.h"
#include "Content.h"
#include "../Debugging/Error.h"

TextureManager::TextureManager(GraphicsDeviceManager& graphicsManager)
{
	// Get the asset path
	m_assetsDir = Content::GetAssetsDir();

	CreateTransparencyRect(graphicsManager);
}

Texture* TextureManager::GetTexture(const std::string& name) const
{
	// Find the texture in the map 
	const auto iter = m_textureMap.find(name);
	if(iter != m_textureMap.end())
	{
		return iter->second.view.Get();
	}
	return nullptr;
}

Vector2 TextureManager::GetDimensions(const std::string& name) const
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	m_textureMap.at(name).texture->GetDesc(&desc);

	return Vector2(static_cast<float>(desc.Width), static_cast<float>(desc.Height));
}

void TextureManager::CreateTransparencyRect(GraphicsDeviceManager& graphicsManager)
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

	ID3D11Texture2D* texture = nullptr;

	float* buffer = new float[static_cast<int>(WINDOW_WIDTH) * static_cast<int>(WINDOW_HEIGHT) * 4];

	for(int y = 0; y < WINDOW_HEIGHT * WINDOW_WIDTH * 4; ++y)
	{
		buffer[y] = 1.0f;
	}

	D3D11_SUBRESOURCE_DATA texData;
	texData.SysMemPitch = static_cast<int>(WINDOW_WIDTH) * sizeof(float) * 4;
	texData.SysMemSlicePitch = static_cast<int>(WINDOW_WIDTH) * static_cast<int>(WINDOW_HEIGHT) * sizeof(float) * 4;
	texData.pSysMem = static_cast<void*>(buffer);

	HRESULT hr = graphicsManager.GetDevice()->CreateTexture2D(&desc, &texData, &texture);
	ASSERT(hr == S_OK);

	delete[] buffer;

	ID3D11ShaderResourceView* resourceView = nullptr;
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;
	viewDesc.Texture2D.MostDetailedMip = 0;

	hr = graphicsManager.GetDevice()->CreateShaderResourceView(texture, &viewDesc, &resourceView);
	ASSERT(hr == S_OK);

	ResourcePair p;
	p.texture = texture;
	p.view = resourceView;
	m_textureMap["transparent"] = p;
}
