#pragma once

namespace Xerxes
{
	namespace Engine
	{
		namespace Graphics
		{
			namespace Device
			{
				class GraphicRenderTarget
				{
				public:
					GraphicRenderTarget();
					GraphicRenderTarget(const GraphicRenderTarget&) = default;
					~GraphicRenderTarget() = default;

					void Initialize(ID3D11Device* device, int textureWidth, int textureHeight);
					void Shutdown();
					void SetRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView, ID3D11RenderTargetView* rtv);

					void ClearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView,
						DirectX::XMVECTORF32 color);

					void ClearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView,
						float red, float green, float blue, float alpha);
					ID3D11ShaderResourceView* GetShaderResourceView();

				private:
					Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_windowRenderTargetTexture;
					Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_windowRenderTargetView;
					Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_windowShaderResourceView;
					Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_d3dDepthStencilView;
					Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_depthStencil;

				};

			}
		}
	}
}