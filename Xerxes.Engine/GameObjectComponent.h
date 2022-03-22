#pragma once

#include <memory>
#include <string>

#include "GameObject.h"

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/ImGuizmo.h"
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
	virtual void OnRender(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* context) = 0;
	virtual void OnStart() = 0;
	virtual void OnAwake() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnGizmo(ImGuizmo::OPERATION manipulationOperation, ImGuizmo::MODE manipulationMode) = 0;
	virtual void OnInspector() = 0;
	virtual void OnDestroy() = 0;
	virtual std::string GetName() = 0;

protected:
	friend class GameObject;
	GameObject*			gameObject;

};

