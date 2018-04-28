//-----------------------------------------------------------
// File: GraphicsDeviceManager.h
// Description: Singleton manages the graphics device, directx 
//				interface initialization and starting and ending
//				rendering
//-----------------------------------------------------------

#ifndef GRAPHICS_DEVICE_MANAGER_H
#define GRAPHICS_DEVICE_MANAGER_H

#include <memory>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include <wrl/client.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "../Utilities/Color.h"

class IWindow;

using Microsoft::WRL::ComPtr;

class GraphicsDeviceManager
{
public:
	
	GraphicsDeviceManager(IWindow* window, bool windowed);

	ID3D11Device* GetDevice() const { return m_pGraphicsDevice.Get(); }
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* GetBatch() const { return primitiveBatch.get(); }

	// Gets the graphics device ready for rendering
	void BeginScene( const Color& backColor );

	// Stops the graphics device rendering
	void EndScene();
	DirectX::SpriteBatch* GetSpriteBatch() const;

private:

	void CreateDeviceOnly();

	void CreateSwapChainAndDevice(IWindow& window);
	void CreateBatchesAndEffect();
	void CreateSpriteFont();
	void CreateBackBuffers();

	// DirectX pointers
	ComPtr<ID3D11Device> m_pGraphicsDevice;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	ComPtr<IDXGISwapChain> m_pSwapChain;
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> primitiveBatch;
	std::unique_ptr<DirectX::BasicEffect> basicEffect;
	std::unique_ptr<DirectX::SpriteFont> font;

	ComPtr<ID3D11InputLayout> m_pInputLayout;

	bool sceneStarted = false;
};


#endif