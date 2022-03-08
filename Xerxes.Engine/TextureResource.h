#pragma once
#include "GameResource.h"

class TextureResource : public GameResource<ID3D11ShaderResourceView>
{
	// Inherited via GameResource
	virtual void OnInspector() override;
};

