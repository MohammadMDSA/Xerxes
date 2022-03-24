#pragma once
#include "IManager.h"
#include "LightComponent.h"
class LightManager : public IManager
{
public:
	LightManager();
	~LightManager();

	LightComponent*				GetDirectionalLight();
	void						SetDirectionalLight(LightComponent* light);
	void						ApplyToEffect(DirectX::IEffectLights* effect);

	// Inherited via IManager
	virtual void				OnInit() override;
	virtual void				OnShutdown() override;

private:

	LightComponent*				light;
};

