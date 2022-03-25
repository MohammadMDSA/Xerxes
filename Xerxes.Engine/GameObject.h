#pragma once

#include "Transform.h"
#include <memory>
#include "IInspectorDrawer.h"

#include "GameObjectComponent.h"
#include "Scene.h"
#include "XPreprocessors.h"

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/ImGuizmo.h"
#include "Libs/EnTT/entt.hpp"
#include "boost/serialization/access.hpp"
#include "boost/serialization/split_member.hpp"

class GameObjectComponent;
class LightComponent;
class MeshRenderer;
class RootManager;

template<typename Type>
Type& getEntityComponent(entt::registry& registry, entt::entity entity) {
	return registry.template get<Type>(entity);
}


class GameObject : public IInspectorDrawer
{
public:

	Transform& transform() const;
	Transform& transform();

	void					OnStart();
	void					OnAwake();
	void					OnUpdate(float deltaTime);
	void					OnRender(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* context);
	void					OnDestroy();

	void					OnGizmo(ImGuizmo::OPERATION manipulationOperation, ImGuizmo::MODE manipulationMode);
	virtual void			OnInspector() override;

	template<class T>
	inline T&						AttachCopiedComponent(const T& copyFrom) {
		auto& comp = GetScene()->registry.emplace<T>(entityId, copyFrom);
		auto& goComp = static_cast<GameObjectComponent&>(comp);
		goComp.gameObject = this;
		if (isStarted)
			goComp.OnStart();
		if (isAwake)
			goComp.OnAwake();
		return comp;
	}

	template<class T>
	T&						AttachComponent();

	template<class T>
	void					DeleteComponent();

	void					SetName(std::string name);
	std::string				GetName();

	template<typename T>
	T&						GetComponent();

	void					SetTransform(const Transform& trans);

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
	std::vector<GameObjectComponent*>	GetComponents() const;

	std::string					name;
	std::string					editName;

	bool						isAwake;
	bool						isStarted;
	bool						destroyed;

	entt::entity				entityId;
	Scene* scene;

	inline Scene* GetScene() { return scene; }

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		ar & name;
		ar & isAwake;
		ar & isStarted;
		ar & entityId;
		ar& scene;

		auto trans = &this->transform();

		ar & trans;

		auto components = GetComponents();
		ar & components.size();

		for (auto comp : components)
		{
			const auto cname = comp->GetName();
			ar& cname;
			ar& (*comp);
			/*if (cname == XNameOf(LightComponent))
			{
				ar& dynamic_cast<LightComponent*>(comp);
			}
			else if (cname == XNameOf(MeshRenderer))
			{
				ar& dynamic_cast<MeshRenderer*>(comp);

			}*/
		}
	}
	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		ar& name;
		ar& isAwake;
		ar& isStarted;
		ar& entityId;
		ar& scene;

		Transform* transform;
		ar& transform;

		GetScene()->registry.emplace_or_replace<Transform>(entityId, *transform);

		int componentCount;
		ar& componentCount;
		for (size_t i = 0; i < componentCount; i++)
		{
			std::string cname;
			ar& cname;
			if (cname == XNameOf(LightComponent))
			{
				LightComponent cmp;
				ar& cmp;
				AttachCopiedComponent<LightComponent>(cmp);
			}
			else if (cname == XNameOf(MeshRenderer))
			{
				MeshRenderer msh;
				ar& msh;
				AttachCopiedComponent<MeshRenderer>(msh);
			}
		}
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
};

