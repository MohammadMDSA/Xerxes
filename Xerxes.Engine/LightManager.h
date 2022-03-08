#pragma once
#include "IManager.h"
#include <unordered_map>
#include "LightComponent.h"
class LightManager : public IManager
{
public:
	LightManager();
	~LightManager();

	int							AddDirectionalLight(LightComponent* light);
	void						RemoveDirectionalLight(LightComponent* light);
	LightComponent*				GetLightById(int id);
	LightComponent*				GetActiveLight();
	void						SetActiveLight(int id);
	void						ApplyToEffect(DirectX::IEffectLights* effect);

	// Inherited via IManager
	virtual void				OnInit() override;
	virtual void				OnShutdown() override;

private:
	int							GetFirstAvailableId();

	std::unordered_map<int, LightComponent*>	lights;

	int							lastId;
	int							activeId;
};

