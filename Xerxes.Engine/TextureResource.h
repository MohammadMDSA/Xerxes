#pragma once
#include "GameResource.h"

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

