#include "pch.h"
#include "LightComponent.h"
#include "RootManager.h"

using namespace DirectX;

LightComponent::LightComponent() :
	color(Colors::Blue),
	specular(Colors::White)
{
	RootManager::GetInstance()->GetLightManager()->AddDirectionalLight(this);
}

LightComponent::~LightComponent()
{
}

void LightComponent::OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, ID3D11DeviceContext* context)
{
}

void LightComponent::OnStart()
{
}

void LightComponent::OnAwake()
{
}

void LightComponent::OnUpdate(float deltaTime)
{
}

void LightComponent::OnGizmo()
{
}

void LightComponent::OnInspector()
{
	ImGui::Text("Color");
	ImGui::SameLine();
	ImGui::ColorEdit4("Color", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
	ImGui::Text("Specular");
	ImGui::SameLine();
	ImGui::ColorEdit4("Specular", (float*)&specular, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
}

void LightComponent::OnDestroy()
{
}

std::string LightComponent::GetName()
{
	return "Light";
}

void LightComponent::SetColor(DirectX::SimpleMath::Vector4 color)
{
	this->color = color;
}

void LightComponent::SetSpecular(DirectX::SimpleMath::Vector4 color)
{
	this->specular = color;
}

void LightComponent::SetIndirectMultiplier(float coef)
{
	this->indirectMultiplier = coef;
}

void LightComponent::SetIntencity(float intencity)
{
	this->intencity = intencity;
}

DirectX::SimpleMath::Vector4 LightComponent::GetColor()
{
	return color;
}

DirectX::SimpleMath::Vector4 LightComponent::GetSpecular()
{
	return specular;
}

DirectX::SimpleMath::Vector3 LightComponent::GetDirection()
{
	return gameObject->transform.Forward();
}

float LightComponent::GetIndirectMultiplier()
{
	return indirectMultiplier;
}

float LightComponent::GetIntencity()
{
	return intencity;
}
