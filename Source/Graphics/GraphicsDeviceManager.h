//-----------------------------------------------------------
// File: GraphicsDeviceManager.h
// Description: Singleton manages the graphics device, directx 
//				interface initialization and starting and ending
//				rendering
//-----------------------------------------------------------

#ifndef GRAPHICS_DEVICE_MANAGER_H
#define GRAPHICS_DEVICE_MANAGER_H

#include "../UserInterface/Window.h"
#include <memory>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

class GraphicsDeviceManager
{
public:
	GraphicsDeviceManager(Window& window, bool windowed);

	ID3D11Device* GetDevice() const { return m_pGraphicsDevice.Get(); }
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* GetBatch() const { return primitiveBatch.get(); }

	// Gets the graphics device ready for rendering
	void BeginScene( const Color& backColor );

	// Stops the graphics device rendering
	void EndScene();
	DirectX::SpriteBatch* GetSpriteBatch() const;

private:
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