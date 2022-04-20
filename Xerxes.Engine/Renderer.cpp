#include "pch.h"
#include "Renderer.h"

using namespace DirectX;
using namespace DX;
using namespace Xerxes::Engine::Graphics::Device;

using Microsoft::WRL::ComPtr;

void Xerxes::Engine::SubsystemManager::Renderer::OnInit()
{
}

void Xerxes::Engine::SubsystemManager::Renderer::OnShutdown()
{
}

void Xerxes::Engine::SubsystemManager::Renderer::OnDeviceLost()
{
	for (auto notListener : notifyListeners)
	{
		try
		{
			notListener->OnDeviceLost();
		}
		catch (const std::exception&)
		{

		}
	}
}

void Xerxes::Engine::SubsystemManager::Renderer::OnDeviceRestored()
{
	for (auto notListener : notifyListeners)
	{
		try
		{
			notListener->OnDeviceLost();
		}
		catch (const std::exception&)
		{

		}
	}
}
