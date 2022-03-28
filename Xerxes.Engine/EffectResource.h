#pragma once
#include "GameResource.h"
#include "Effects.h"

class EffectResource : public GameResource<DirectX::IEffect>
{
public:
	EffectResource();
	EffectResource(const EffectResource& other) = default;
	~EffectResource() = default;

	virtual void OnInspector() override;
	ID3D11InputLayout* GetInputLayout();

	void CreateInputLayout(ID3D11Device* device);

	virtual void Initialize(ID3D11DeviceContext* device) override;
	virtual void Shutdown() override;

	inline const std::string& GetVertextType() const { return vertexType; }
	inline void SetVertexType(std::string type) { vertexType = type; }

	inline void SetVertextColorEnabled(bool enabled) { vertextColorEnabled = enabled; }

	// Enums

	// Effect types enums
	static const std::string XEffectResourceType_Basic;
	static const std::string XEffectResourceType_NormalMap;
	static const std::string XEffectResourceType_Particle;

	// Effect vertext input type enums
	static const std::string XEffectVertexType_VertexPositionColor;
	static const std::string XEffectVertexType_VertexPositionColorTexture;
	static const std::string XEffectVertexType_VertexPositionOffsetColorTexture;
	static const std::string XEffectVertexType_VertexPositionNormal;
	static const std::string XEffectVertexType_VertexPositionNormalTexture;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	std::string vertexType;

	DirectX::SimpleMath::Vector4 diffuseColor;
	DirectX::SimpleMath::Vector3 emmisiveColor;
	DirectX::SimpleMath::Vector3 specularColor;
	float specularPower;

	bool vertextColorEnabled = false;

};

