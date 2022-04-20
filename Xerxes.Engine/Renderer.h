#pragma once
#include "IManager.h"
#include "DeviceResources.h"

using namespace Xerxes::Engine::Graphics::Device;

namespace Xerxes
{
	namespace Engine
	{
		namespace SubsystemManager
		{

			class Renderer : public IManager, public IDeviceNotify
			{
			public:
				virtual void OnInit() override;
				virtual void OnShutdown() override;

				// Inherited via IDeviceNotify
				virtual void OnDeviceLost() override;

				virtual void OnDeviceRestored() override;

				void RegisterDeviceNotify(IDeviceNotify* deviceNotify) noexcept { notifyListeners.insert(deviceNotify); }

			private:
				std::unordered_set<IDeviceNotify*>	notifyListeners;
			};
		}
	}
}