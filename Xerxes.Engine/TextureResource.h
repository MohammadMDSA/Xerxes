#pragma once
#include "GameResource.h"

#define XDrawTextureInspect(fidName, fName) \
{ \
	auto resourceManager = XResourceM(); \
	auto availableWidth = ImGui::GetContentRegionAvail().x; \
	auto texResource = resourceManager->ResourceGroup<TextureResource>::GetById(fidName); \
	std::string resourceName = texResource ? texResource->GetName() : "[select texture]"; \
	if(ImGui::Button(resourceName.c_str(), ImVec2(0.6f * availableWidth, 0))) \
		ImGui::OpenPopup(#fName "TextureSelection"); \
	ImGui::SameLine(); \
	ImGui::Text(#fName ""); \
	if (ImGui::BeginPopup(#fName "TextureSelection"))\
	{\
		auto textures = resourceManager->ResourceGroup<TextureResource>::GetAll(); \
		if (ImGui::Selectable("<none>")) \
		{ \
			fidName = -1; \
		} \
		int i = 0;\
		for (auto it : textures) \
		{ \
			ImGui::PushID(++i); \
			if (ImGui::Selectable((it->GetName() + " (" + it->GetType() + ")").c_str())) \
			{ \
				fidName = it->GetId(); \
			} \
			ImGui::PopID(); \
		} \
		ImGui::EndPopup(); \
	} \
	ImGui::Spacing(); \
}


class TextureResource : public GameResource<ID3D11ShaderResourceView>
{
public:
	TextureResource();
	TextureResource(const TextureResource& const) = default;
	~TextureResource() = default;

	// Inherited via GameResource
	virtual void OnInspector() override;

	// Inherited via GameResource
	virtual void Initialize(ID3D11DeviceContext* context) override;
	virtual void Shutdown() override;

	virtual ID3D11ShaderResourceView* GetResource() override;
	virtual ID3D11ShaderResourceView** GetResourceAddress();

	// Enums

	// Model type enums
	static const std::string XTextureResourceType_DDS;
	static const std::string XTextureResourceType_BMP;
private:
	friend class ParticleSystem;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> resource;
};

