#include "pch.h"
#include "EffectResource.h"
#include "Libs/imgui/imgui.h"

using namespace DirectX;

const std::string EffectResource::XEffectResourceType_Basic = "BasicEffect";
const std::string EffectResource::XEffectResourceType_NormalMap = "NormalMapEffect";
const std::string EffectResource::XEffectVertexType_VertexPositionColor = "VertexPositionColor";
const std::string EffectResource::XEffectVertexType_VertexPositionNormalTexture = "VertexPositionNormalTexture";

EffectResource::EffectResource() :
	diffuseColor(DirectX::SimpleMath::Vector4(1.f, 1.f, 1.f, 1.f)),
	specularColor(DirectX::SimpleMath::Vector3(1.f, 1.f, 1.f)),
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

	ImGui::Text("Specular Color:");
	ImGui::SameLine();
	if (ImGui::ColorEdit3("Specular Color", (float*)&specularColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
	{
		//basicEff->SetSpecularColor(specularColor);
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
		DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionNormalTexture>(device, effect, inputLayout.ReleaseAndGetAddressOf());
	else if (vertexType == XEffectVertexType_VertexPositionColor)
		DirectX::CreateInputLayoutFromEffect<DirectX::VertexPositionColor>(device, effect, inputLayout.ReleaseAndGetAddressOf());
}

void EffectResource::Initialize(ID3D11DeviceContext* context)
{
	if (resource)
		return;
	ID3D11Device* device;
	context->GetDevice(&device);
	if (type == XEffectResourceType_Basic)
		resource = std::make_unique<BasicEffect>(device);
	else if (type == XEffectResourceType_NormalMap)
		resource = std::make_unique<NormalMapEffect>(device);
	if (auto basic = dynamic_cast<BasicEffect*>(resource.get()); basic)
	{
		basic->SetVertexColorEnabled(vertextColorEnabled);
	}
	CreateInputLayout(device);
}

void EffectResource::Shutdown()
{
	resource.release();
	inputLayout.Reset();
}
