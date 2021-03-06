#include "pch.h"
#include "LightComponent.h"
#include "RootManager.h"

using namespace DirectX;
using namespace entt::literals;

XCOMP_GENERATE_DEFINITION(LightComponent)

LightComponent::LightComponent() :
	GameObjectComponent(XNameOf(LightComponent)),
	color(Colors::White),
	ambientIntencity(.2)
{
	XCOMP_GENERATE_CONSTRUCTOR(LightComponent)
	RootManager::GetInstance()->GetLightManager()->SetDirectionalLight(this);
}

LightComponent::LightComponent(const LightComponent& other) :
	GameObjectComponent(other)
{
	this->color = other.color;
}

LightComponent::~LightComponent()
{
}

void LightComponent::OnRender(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* context)
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

void LightComponent::OnGizmo(ImGuizmo::OPERATION manipulationOperation, ImGuizmo::MODE manipulationMode)
{
}

void LightComponent::OnInspector()
{
	ImGui::Text("Color:");
	ImGui::SameLine();
	ImGui::ColorEdit3("Color", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha);

	ImGui::DragFloat("Intencity", &ambientIntencity, 0.01, 0, 1);
}

void LightComponent::OnDestroy()
{
}

const DirectX::SimpleMath::Vector3& LightComponent::GetColor() const
{
	return color;
}

const DirectX::SimpleMath::Vector3& LightComponent::GetDirection() const
{
	return gameObject->transform().Forward();
}

const float& LightComponent::GetAmbientIntencity() const
{
	return this->ambientIntencity;
}

void LightComponent::SetColor(DirectX::SimpleMath::Vector3 color)
{
	this->color = color;
}

void LightComponent::SetAmbientIntencity(float intencity)
{
	this->ambientIntencity = intencity;
}
