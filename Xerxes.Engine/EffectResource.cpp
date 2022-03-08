#include "pch.h"
#include "EffectResource.h"
#include "Libs/imgui/imgui.h"

using namespace DirectX;

void EffectResource::OnInspector()
{
	auto res = resource.get();
	ImGui::Text("Diffuse Color:");
	ImGui::SameLine();
	if (ImGui::ColorEdit4("Diffuse Color", (float*)&DiffuseColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
	{
		res->SetLightDiffuseColor(0, DiffuseColor);
	}

}
