#pragma once
#include "GameResource.h"
#include "Effects.h"

class EffectResource : public GameResource<DirectX::IEffect>
{
	// Inherited via GameResource
	virtual void OnInspector() override;
};

