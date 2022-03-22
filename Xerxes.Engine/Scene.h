#pragma once
#include "GameObject.h"
#include "Libs/EnTT/entt.hpp"
#include "GameObjectComponent.h"
#include <unordered_map>

class Scene
{
public:
	Scene(const Scene& other) = default;

	std::vector<GameObject*>		GetGameObjects();
	entt::entity					AddGameObject(GameObject* obj);

private:
	friend class SceneManager;
	friend class GameObject;
	Scene();
	~Scene() = default;

	entt::registry			registry;
	std::unordered_map<entt::entity, GameObject*>	objects;
};

