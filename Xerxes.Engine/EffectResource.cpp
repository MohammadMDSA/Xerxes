#include "pch.h"
#include "EffectResource.h"
#include "ParticleEffect.h"
#include "VertexPositionOffsetColorTexture.h"
#include "Libs/imgui/imgui.h"

using namespace DirectX;
using namespace Xerxes::Engine::Graphics::Effects;
using namespace Xerxes::Engine::Graphics::VertexTypes;

const std::string EffectResource::XEffectResourceType_Basic = "BasicEffect";
const std::string EffectResource::XEffectResourceType_NormalMap = "NormalMapEffect";
const std::string EffectResource::XEffectResourceType_Particle = "ParticleEffect";
const std::string EffectResource::XEffectVertexType_VertexPositionColor = "VertexPositionColor";
const std::string EffectResource::XEffectVertexType_VertexPositionColorTexture = "VertexPositionColorTexture";
const std::string EffectResource::XEffectVertexType_VertexPositionOffsetColorTexture = "VertexPositionOffsetColorTexture";
const std::string EffectResource::XEffectVertexType_VertexPositionNormal = "VertexPositionNormal";
const std::string EffectResource::XEffectVertexType_VertexPositionNormalTexture = "VertexPositionNormalTexture";

EffectResource::EffectResource() :
	diffuseColor(DirectX::SimpleMath::Vector4(1.f, 1.f, 1.f, 1.f)),
	specularPower(16),
	vertexType(XEffectVertexType_VertexPositionNormalTexture)
{
}

void EffectResource::OnInspector()
{
	auto res = resource.get();
	ImGui::Text("Diffuse Color:");
	ImGui::SameLine();
	auto basicEff = dynamic_cast<BasicEffect*>(res);
	if (ImGui::ColorEdit4("Diffuse Color", (float*)&diffuseColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
	{
		basicEff->SetColorAndAlpha(diffuseColor);
	}

	ImGui::Text("Emmisive Color:");
	ImGui::SameLine();
	if (ImGui::ColorEdit3("Emmisive Color", (float*)&emmisiveColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha))
	{
		basicEff->SetEmissiveColor(emmisiveColor);
	}

	ImGui::Text("Smoothness:");
	ImGui::SameLine();
	if (ImGui::SliderFloat("Specular Power", (float*)&specularPower, 0, 1, "%.3f", ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
	{
		basicEff->SetSpecularPower(specularPower * 1000 + 0.01);
	}


}

ID3D11InputLayout* EffectResource::GetInputLayout()
{
	return inputLayout.Get();
}

void EffectResource::CreateInputLayout(ID3D11Device* device)
{
	auto effect = resource.get();
	if (vertexType == XEffectVertexType_VertexPositionNormalTexture)
		DX::ThrowIfFailed(DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionNormalTexture>(device, effect, inputLayout.ReleaseAndGetAddressOf()));
	else if (vertexType == XEffectVertexType_VertexPositionColor)
		DX::ThrowIfFailed(DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionColor>(device, effect, inputLayout.ReleaseAndGetAddressOf()));
	else if (vertexType == XEffectVertexType_VertexPositionNormal)
		DX::ThrowIfFailed(DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionNormal>(device, effect, inputLayout.ReleaseAndGetAddressOf()));
	else if (vertexType == XEffectVertexType_VertexPositionColorTexture)
		DX::ThrowIfFailed(DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionColorTexture>(device, effect, inputLayout.ReleaseAndGetAddressOf()));
	else if (vertexType == XEffectVertexType_VertexPositionOffsetColorTexture)
		DX::ThrowIfFailed(DirectX::CreateInputLayoutFromEffect< VertexPositionOffsetColorTexture>(device, effect, inputLayout.ReleaseAndGetAddressOf()));
}

void EffectResource::Initialize(ID3D11DeviceContext* context)
{
	if (loaded)
		return;
	ID3D11Device* device;
	context->GetDevice(&device);
	if (type == XEffectResourceType_Basic)
		resource = std::make_unique<BasicEffect>(device);
	else if (type == XEffectResourceType_NormalMap)
		resource = std::make_unique<NormalMapEffect>(device);
	else if (type == XEffectResourceType_Particle)
		resource = std::make_unique<ParticleEffect>(device);
	if (auto basic = dynamic_cast<BasicEffect*>(resource.get()); basic)
	{
		basic->SetVertexColorEnabled(vertextColorEnabled);
	}

	auto light = dynamic_cast<IEffectLights*>(resource.get());
	if (!vertextColorEnabled && light)
	{
		light->SetLightingEnabled(true);
		light->SetLightEnabled(0, true);
		SetVertexType(XEffectVertexType_VertexPositionNormal);
	}

	CreateInputLayout(device);
	loaded = true;
}

void EffectResource::Shutdown()
{
	loaded = false;
	resource.release();
	inputLayout.Reset();
}
