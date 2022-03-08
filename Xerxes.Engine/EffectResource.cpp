#include "pch.h"
#include "EffectResource.h"
#include "Libs/imgui/imgui.h"

using namespace DirectX;

void EffectResource::OnInspector()
{
	auto res = resource.get();
	ImGui::Text("Diffuse Color:");
	ImGui::SameLine();
	if (ImGui::ColorEdit4("Diffuse Color", (float*)&diffuseColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
	{
		res->SetColorAndAlpha(diffuseColor);
	}

}
