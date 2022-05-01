#include "pch.h"
#include "LightComponent.h"
#include "RootManager.h"
#include <boost/serialization/export.hpp>

using namespace DirectX;
using namespace entt::literals;

XCOMP_GENERATE_DEFINITION(LightComponent)

RTTR_REGISTRATION
{
	rttr::registration::class_<LightComponent>(XNameOf(LightComponent))
		.constructor<>()
		XRegisterProperty(color, LightComponent);
}

LightComponent::LightComponent() :
	GameObjectComponent(),
	color(Colors::White),
	ambientIntencity(.2f)
{
	XCOMP_GENERATE_CONSTRUCTOR(LightComponent)
	RootManager::GetInstance()->GetLightManager()->SetDirectionalLight(this);
}

LightComponent::LightComponent(const LightComponent& other) :
	GameObjectComponent(other)
{
	this->gameObject = other.gameObject;
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

	ImGui::DragFloat("Intencity", &ambientIntencity, 0.01f, 0.f, 1.f);
}

void LightComponent::OnDestroy()
{
}

const DirectX::SimpleMath::Vector3& LightComponent::GetDirection() const
{
	return gameObject->transform().Forward();
}

LightComponent& LightComponent::operator=(const LightComponent& other)
{
	this->gameObject = other.gameObject;
	this->color = other.color;
	return *this;
}
