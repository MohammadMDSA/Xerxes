#pragma once
#include "GameResource.h"

class ModelResource : public GameResource<DirectX::Model>
{
	// Inherited via GameResource
	virtual void OnInspector() override;
};

