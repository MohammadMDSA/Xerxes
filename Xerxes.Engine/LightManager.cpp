#include "pch.h"
#include "LightManager.h"
#include <exception>

LightManager::LightManager()
{
}

LightManager::~LightManager()
{
}

int LightManager::AddDirectionalLight(LightComponent* light)
{
	for (auto l : lights)
	{
		if (l.second == light)
			return l.first;
	}
	lights.insert({ ++lastId, light });
	if (activeId < 0)
		activeId = lastId;
	return lastId;
}

void LightManager::RemoveDirectionalLight(LightComponent* light)
{
	//std::remove_if(lights.begin(), lights.end(), []);
}

LightComponent* LightManager::GetLightById(int id)
{
	if (!lights.contains(id))
		throw std::exception("Key doesn\'t exists.");
	return lights[id];
}

LightComponent* LightManager::GetActiveLight()
{
	if (activeId >= 0 && lights.contains(activeId))
		return lights[activeId];
	return nullptr;
}

void LightManager::SetActiveLight(int id)
{
	if (lights.contains(id))
	{
		activeId = id;
	}
}

void LightManager::ApplyToEffect(DirectX::IEffectLights* effect)
{
	auto light = GetActiveLight();
	if (!light && !dynamic_cast<DirectX::NormalMapEffect*>(effect))
	{
		effect->SetLightingEnabled(false);
		return;
	}
	else if (!light)
		return;
	auto color = light->GetColor();
	effect->SetAmbientLightColor(color);
	effect->SetLightDiffuseColor(0, color);
	effect->SetLightDirection(0, light->GetDirection());
	effect->SetLightEnabled(0, true);
	effect->SetLightingEnabled(true);
	effect->SetLightSpecularColor(0, light->GetSpecular());
}

void LightManager::OnInit()
{
	lastId = -1;
	activeId = -1;
}

void LightManager::OnShutdown()
{
	for (auto& it : lights) {
		delete it.second;
	}
	lights.clear();
}

int LightManager::GetFirstAvailableId()
{
	if (lights.size() <= 0)
		return -1;

	return lights.cbegin()->first;
}