#pragma once
#include "GameResource.h"
#include "Effects.h"

class EffectResource : public GameResource<DirectX::BasicEffect>
{
	// Inherited via GameResource
	virtual void OnInspector() override;

private:
	DirectX::SimpleMath::Vector4 diffuseColor = DirectX::SimpleMath::Vector4(1.f, 1.f, 1.f, 1.f);
};

