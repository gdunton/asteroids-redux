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

class GraphicsDeviceManager : public Singleton<GraphicsDeviceManager>
{
public:
	// Allow the singleton access to the private constructor
	friend Singleton<GraphicsDeviceManager>;
private:
	GraphicsDeviceManager();
	~GraphicsDeviceManager();

	// DirectX pointers
	ID3D11Device* m_pGraphicsDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;


	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> primitiveBatch;
	std::unique_ptr<DirectX::BasicEffect> basicEffect;

	ID3D11InputLayout* m_pInputLayout;

	bool sceneStarted;

public:

	bool Initialize( Window& window, const bool windowed );

	ID3D11Device* GetDevice() { return m_pGraphicsDevice; }
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* GetBatch() { return primitiveBatch.get(); }

	// Gets the graphics device ready for rendering
	void BeginScene( const Color& backColor );

	// Stops the graphics device rendering
	void EndScene();
};


#endif