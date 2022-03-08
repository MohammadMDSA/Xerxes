#pragma once
#include "GameResource.h"

class GeometricPrimitiveResource : public GameResource<DirectX::GeometricPrimitive>
{
	// Inherited via GameResource
	virtual void OnInspector() override;
};

