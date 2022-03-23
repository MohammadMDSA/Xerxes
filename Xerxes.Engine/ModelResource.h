#pragma once
#include "GameResource.h"


class ModelResource : public GameResource<DirectX::Model>
{
public:

	// Inherited via GameResource
	virtual void OnInspector() override;

	// Inherited via GameResource
	virtual void Initialize(ID3D11DeviceContext* context) override;
	virtual void Shutdown() override;

	// Enums

	// Model type enums
	static const std::string XModelResourceType_SDKMESH;


private:
	void LoadSDKMESHModel(ID3D11Device* device);
};

