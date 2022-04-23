#pragma once
#include "IManager.h"
#include "IDeviceNotify.h"
#include "GraphicRenderTarget.h"

using namespace Xerxes::Engine::Graphics::Device;
using namespace DirectX;

namespace Xerxes
{
	namespace Engine
	{
		namespace SubsystemManagers
		{

			class Renderer : public IManager, public IDeviceNotify
			{
			public:
				virtual void OnInit() override;
				virtual void OnShutdown() override;

				// Inherited via IDeviceNotify
				virtual void OnDeviceLost() override;

				virtual void OnDeviceRestored(ID3D11DeviceContext* context, ID3D11Device* device) override;

				void RegisterDeviceNotify(IDeviceNotify* deviceNotify) noexcept { notifyListeners.insert(deviceNotify); }

				void SetWindow(HWND window, int width, int height) noexcept;

				ID3D11Device* GetDevice();
				ID3D11DeviceContext* GetDeviceContext();
				inline ID3D11ShaderResourceView* GetViewTexture() { return renderTarget->GetShaderResourceView(); }
				
				void SetRenderDimansion(int width, int height);
				void GetRenderDimansion(int& width, int& height);
				void Present();
				bool SetOutputSize(int width, int height);

				void Render();
				void Clear();

				void CreateResource();

			private:
				class DeviceResources;

				std::unordered_set<IDeviceNotify*>	notifyListeners;
				DeviceResources*					deviceResource;
				std::unique_ptr<GraphicRenderTarget> renderTarget;
				int renderWidth, renderHeight;
			};
		}
	}
}