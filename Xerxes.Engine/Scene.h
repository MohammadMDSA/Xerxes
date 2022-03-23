#pragma once
#include "GameObject.h"
#include "Libs/EnTT/entt.hpp"
#include "GameObjectComponent.h"
#include <unordered_map>

#include "boost/serialization/access.hpp"
#include "boost/serialization/split_member.hpp"

class Scene
{
public:
	Scene(const Scene& other) = default;

	std::vector<GameObject*>		GetGameObjects();
	GameObject*						CreateGameObject();
	GameObject*						Instantiate(const GameObject& other);
	void							RemoveGameObject(GameObject* gameObejct);

private:
	friend class SceneManager;
	friend class GameObject;
	friend class boost::serialization::access;
	Scene();
	~Scene();

	entt::registry			registry;
	std::unordered_map<entt::entity, std::unique_ptr<GameObject>>	objects;
	std::string				name;

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		ar& objects.size();
		for (auto& pair : objects)
		{
			ar& pair.first;
			ar& pair.second.get();
		}
		ar& name;
	}

    template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		int size;
		ar& size;

		for (size_t i = 0; i < size; i++)
		{
			entt::entity key;
			GameObject* obj;

			ar& key;
			ar& obj;
			objects[key] = std::unique_ptr<GameObject>(obj);
			//obj->scene = this;
		}
		ar& name;
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()
};
