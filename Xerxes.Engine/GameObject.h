#pragma once

#include "Transform.h"
#include <memory>
#include "IInspectorDrawer.h"

#include "GameObjectComponent.h"
#include "Scene.h"

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/ImGuizmo.h"
#include "Libs/EnTT/entt.hpp"

class GameObjectComponent;
class Scene;

template<typename Type>
Type& getEntityComponent(entt::registry& registry, entt::entity entity) {
	return registry.template get<Type>(entity);
}


class GameObject : public IInspectorDrawer
{
public:
	GameObject(Scene* scene);
	~GameObject();

	Transform transform;

	void					OnStart();
	void					OnAwake();
	void					OnUpdate(float deltaTime);
	void					OnRender(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* context);
	void					OnDestroy();

	void					OnGizmo();
	virtual void			OnInspector() override;

	template<class T>
	T&						AttachComponent();

	template<class T>
	void					DeleteComponent();

	void					SetName(std::string name);
	std::string				GetName();

private:

	std::vector<GameObjectComponent*>	GetComponents();

	ImGuizmo::OPERATION			manipulationOperation;
	ImGuizmo::MODE				manipulationMode;
	std::string					name;
	std::string					editName;

	bool						isAwake;
	bool						isStarted;

	entt::entity				entityId;
	Scene*						scene;
};

