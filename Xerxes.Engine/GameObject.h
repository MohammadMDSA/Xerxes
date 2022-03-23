#pragma once

#include "Transform.h"
#include <memory>
#include "IInspectorDrawer.h"

#include "GameObjectComponent.h"
#include "Scene.h"

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/ImGuizmo.h"
#include "Libs/EnTT/entt.hpp"
#include "boost/serialization/access.hpp"

class GameObjectComponent;
class Scene;

template<typename Type>
Type& getEntityComponent(entt::registry& registry, entt::entity entity) {
	return registry.template get<Type>(entity);
}


class GameObject : public IInspectorDrawer
{
public:

	Transform&				transform();

	void					OnStart();
	void					OnAwake();
	void					OnUpdate(float deltaTime);
	void					OnRender(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* context);
	void					OnDestroy();

	void					OnGizmo(ImGuizmo::OPERATION manipulationOperation, ImGuizmo::MODE manipulationMode);
	virtual void			OnInspector() override;

	template<class T>
	T&						AttachComponent();

	template<class T>
	void					DeleteComponent();

	void					SetName(std::string name);
	std::string				GetName();

	template<typename T>
	T&						GetComponent();

	static GameObject*		Create();
	static void				Destroy(GameObject* obj);

private:
	friend class boost::serialization::access;
	friend struct std::default_delete<GameObject>;
	friend class Scene;

	GameObject() = default;
	GameObject(Scene* scene);
	GameObject(const GameObject& other);
	~GameObject() = default;
	std::vector<GameObjectComponent*>	GetComponents();

	std::string					name;
	std::string					editName;

	bool						isAwake;
	bool						isStarted;
	bool						destroyed;

	entt::entity				entityId;
	Scene*						scene;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {}
};

