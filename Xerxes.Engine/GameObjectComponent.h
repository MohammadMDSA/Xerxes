#pragma once

#include <memory>
#include <string>

#include "GameObject.h"
#include "Libs/EnTT/entt.hpp"

#define XCOMP_REGISTER(COMP) \
{ \
	entt::meta<COMP>().type() \
		.func<&COMP::OnRender>("OnRender"_hs) \
		.func<&COMP::OnStart>("OnStart"_hs) \
		.func<&COMP::OnAwake>("OnAwake"_hs) \
		.func<&COMP::OnUpdate>("OnUpdate"_hs) \
		.func<&COMP::OnGizmo>("OnGizmo"_hs) \
		.func<&COMP::OnInspector>("OnInspector"_hs) \
		.func<&COMP::OnDestroy>("OnDestroy"_hs) \
		.func<&getEntityComponent<COMP>, entt::as_ref_t>("get"_hs); \
}

#define XCOMP_GENERATE_BODY() \
private: \
static bool initialized;

#define XCOMP_GENERATE_DEFINITION(T) \
bool T::initialized = false;

#define XCOMP_GENERATE_CONSTRUCTOR(T) \
{ \
	if(!initialized) \
	{ \
		initialized = true; \
		XCOMP_REGISTER(T) \
	} \
}

class GameObject;

class GameObjectComponent : public IInspectorDrawer
{
public:
	virtual void OnRender(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* context) {};
	virtual void OnStart() {};
	virtual void OnAwake() {};
	virtual void OnUpdate(float deltaTime) {};
	virtual void OnGizmo() {};
	virtual void OnInspector() {};
	virtual void OnDestroy() {};
	virtual std::string GetName() { return ""; };

protected:
	friend class GameObject;
	GameObject*			gameObject;

};

