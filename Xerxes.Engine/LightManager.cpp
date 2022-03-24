#include "pch.h"
#include "LightManager.h"
#include <exception>

LightManager::LightManager() :
	light(nullptr)
{
}

LightManager::~LightManager()
{
}

LightComponent* LightManager::GetDirectionalLight()
{
	return light;
}

void LightManager::SetDirectionalLight(LightComponent* light)
{
	this->light = light;
}

void LightManager::ApplyToEffect(DirectX::IEffectLights* effect)
{
	auto light = GetDirectionalLight();
	auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
	if (!light)
	{
		effect->EnableDefaultLighting();
		return;
	}
	auto color = light->GetColor();
	effect->SetLightingEnabled(true);
	effect->SetLightEnabled(0, true);
	effect->SetLightDiffuseColor(0, color);
	effect->SetLightDirection(0, light->GetDirection());
	effect->SetLightSpecularColor(0, color);
	effect->SetAmbientLightColor(color * 0.1);
}

void LightManager::OnInit()
{
	light = nullptr;
}

void LightManager::OnShutdown()
{
}
