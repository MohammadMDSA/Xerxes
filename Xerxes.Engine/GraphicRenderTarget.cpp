#include "pch.h"
#include "GraphicRenderTarget.h"
#include "XPreprocessors.h"

Xerxes::Engine::Graphics::Device::GraphicRenderTarget::GraphicRenderTarget()
{
	m_windowRenderTargetTexture.Reset();
	m_windowRenderTargetView.Reset();
	m_windowShaderResourceView.Reset();
	m_depthStencil.Reset();
	m_d3dDepthStencilView.Reset();
}

void Xerxes::Engine::Graphics::Device::GraphicRenderTarget::Initialize(ID3D11Device* device, int textureWidth, int textureHeight)
{
	textureWidth = xmax(textureWidth, 2);
	textureHeight = xmax(textureHeight, 2);
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	CD3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = textureWidth;
	depthStencilDesc.Height = textureHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	DX::ThrowIfFailed(device->CreateTexture2D(
		&depthStencilDesc,
		nullptr,
		m_depthStencil.ReleaseAndGetAddressOf()
	));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D24_UNORM_S8_UINT);
	DX::ThrowIfFailed(device->CreateDepthStencilView(
		m_depthStencil.Get(),
		&depthStencilViewDesc,
		m_d3dDepthStencilView.ReleaseAndGetAddressOf()
	));

	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target texture.
	result = device->CreateTexture2D(&textureDesc, NULL, m_windowRenderTargetTexture.ReleaseAndGetAddressOf());
	DX::ThrowIfFailed(result);

	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = device->CreateRenderTargetView(m_windowRenderTargetTexture.Get(), &renderTargetViewDesc, m_windowRenderTargetView.ReleaseAndGetAddressOf());
	DX::ThrowIfFailed(result);

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	result = device->CreateShaderResourceView(m_windowRenderTargetTexture.Get(), &shaderResourceViewDesc, m_windowShaderResourceView.ReleaseAndGetAddressOf());
	DX::ThrowIfFailed(result);

}

void Xerxes::Engine::Graphics::Device::GraphicRenderTarget::Shutdown()
{
	m_windowRenderTargetTexture.Reset();
	m_windowRenderTargetView.Reset();
	m_windowShaderResourceView.Reset();
}

void Xerxes::Engine::Graphics::Device::GraphicRenderTarget::SetRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView, ID3D11RenderTargetView* rtv)
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, m_windowRenderTargetView.GetAddressOf(), m_d3dDepthStencilView.Get());



	return;
}

void Xerxes::Engine::Graphics::Device::GraphicRenderTarget::ClearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView, DirectX::XMVECTORF32 color)
{
	ClearRenderTarget(deviceContext, depthStencilView, color[0], color[1], color[2], color[3]);
}

void Xerxes::Engine::Graphics::Device::GraphicRenderTarget::ClearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView,
	float red, float green, float blue, float alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	deviceContext->ClearRenderTargetView(m_windowRenderTargetView.Get(), color);

	// Clear the depth buffer.
	deviceContext->ClearDepthStencilView(m_d3dDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

ID3D11ShaderResourceView* Xerxes::Engine::Graphics::Device::GraphicRenderTarget::GetShaderResourceView()
{
	return m_windowShaderResourceView.Get();
}