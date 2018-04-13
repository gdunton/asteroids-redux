//-----------------------------------------------------------
// File: GraphicsDeviceManager.cpp
//-----------------------------------------------------------

#include "STD.h"
#include "GraphicsDeviceManager.h"

#include "../Graphics/Line.h"
#include <CommonStates.h>
#include "../GameMain/Globals.h"
#include <DirectXColors.h>

template<> GraphicsDeviceManager* Singleton<GraphicsDeviceManager>::instance = nullptr;

GraphicsDeviceManager::GraphicsDeviceManager()
{
	m_pGraphicsDevice = nullptr; 
	m_pDeviceContext = nullptr;
	m_pSwapChain = nullptr;
	m_pInputLayout = nullptr;
	m_pRenderTargetView = nullptr;
	m_pDepthStencilView = nullptr;
	sceneStarted = false;
}

GraphicsDeviceManager::~GraphicsDeviceManager()
{
	if( m_pGraphicsDevice ) m_pGraphicsDevice->Release();
	m_pGraphicsDevice = nullptr;
	if (m_pDeviceContext) m_pDeviceContext->Release();
	m_pDeviceContext = nullptr;
	if (m_pSwapChain) m_pSwapChain->Release();
	m_pSwapChain = nullptr;
	if (m_pInputLayout) m_pInputLayout->Release();
	m_pInputLayout = nullptr;
	if (m_pRenderTargetView) m_pRenderTargetView->Release();
	m_pRenderTargetView = nullptr;
	if (m_pDepthStencilView) m_pDepthStencilView->Release();
	m_pDepthStencilView = nullptr;
}

bool GraphicsDeviceManager::Initialize( Window& window, bool windowed )
{
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(swapDesc));
	swapDesc.BufferDesc.Width = static_cast<unsigned int>(WINDOW_WIDTH);
	swapDesc.BufferDesc.Height = static_cast<unsigned int>(WINDOW_HEIGHT);
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapDesc.BufferCount = 2;
	swapDesc.OutputWindow = window.GetWindowHandle();
	swapDesc.Windowed = TRUE;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0,
		nullptr, 0,
		D3D11_SDK_VERSION,
		&swapDesc,
		&m_pSwapChain,
		&m_pGraphicsDevice,
		nullptr,
		&m_pDeviceContext);

	if (hr != S_OK) return false;
	ASSERT( m_pGraphicsDevice != NULL );

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_pDeviceContext);

	primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(m_pDeviceContext);
	basicEffect = std::make_unique<DirectX::BasicEffect>(m_pGraphicsDevice);
	basicEffect->SetProjection(DirectX::XMMatrixOrthographicOffCenterRH(0,
		WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 1));
	basicEffect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	m_pGraphicsDevice->CreateInputLayout(
		DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
		shaderByteCode,
		byteCodeLength, &m_pInputLayout);

	ID3D11Texture2D* backBuffer;
	m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	m_pGraphicsDevice->CreateRenderTargetView(backBuffer, nullptr, &m_pRenderTargetView);
	backBuffer->Release();

	CD3D11_TEXTURE2D_DESC depthStencilDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, 
		static_cast<unsigned int>(WINDOW_WIDTH), static_cast<unsigned int>(WINDOW_HEIGHT), 1, 1, D3D11_BIND_DEPTH_STENCIL);
	ID3D11Texture2D* depthStencil;
	m_pGraphicsDevice->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencil);
	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	m_pGraphicsDevice->CreateDepthStencilView(depthStencil, &depthStencilViewDesc, &m_pDepthStencilView);
	depthStencil->Release();

	// Sprite font stuff
	std::string assetsDir;
	GetAssetsDir(assetsDir);
	assetsDir += "arial.spritefont";

	std::wstring wStr;
	StringToWString(assetsDir, wStr);

	font = std::make_unique<DirectX::SpriteFont>(m_pGraphicsDevice, wStr.c_str());

	return true;
}

void GraphicsDeviceManager::BeginScene( const Color& backColor )
{
	ASSERT( m_pGraphicsDevice );

	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::Black);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT));
	m_pDeviceContext->RSSetViewports(1, &viewport);

	DirectX::CommonStates states(m_pGraphicsDevice);
	m_pDeviceContext->OMSetBlendState(states.Opaque(), nullptr, 0xFFFFFFFF);
	m_pDeviceContext->OMSetDepthStencilState(states.DepthNone(), 0);
	m_pDeviceContext->RSSetState(states.CullNone());

	basicEffect->Apply(m_pDeviceContext);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	sceneStarted = true;
}

void GraphicsDeviceManager::EndScene()
{
	// Stop rendering and present the back buffer
	ASSERT( sceneStarted );
	m_pSwapChain->Present(1, 0);

	sceneStarted = false;
}

DirectX::SpriteBatch* GraphicsDeviceManager::GetSpriteBatch() 
{
	return spriteBatch.get();
}
