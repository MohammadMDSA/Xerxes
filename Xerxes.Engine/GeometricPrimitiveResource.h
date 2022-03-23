#pragma once
#include "GameResource.h"

class GeometricPrimitiveResource : public GameResource<DirectX::GeometricPrimitive>
{
public:
	// Inherited via GameResource
	virtual void OnInspector() override;

	// Inherited via GameResource
	virtual void Initialize(ID3D11DeviceContext* context) override;
	virtual void Shutdown() override;
};

