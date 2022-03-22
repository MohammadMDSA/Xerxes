#include "pch.h"
#include "Scene.h"

using namespace entt;

std::vector<GameObject*> Scene::GetGameObjects()
{
	std::vector<GameObject*> result;
	registry.each([&result, this](entity ent)
		{
			result.push_back(objects[ent]);
		});
	return result;
}

entity Scene::AddGameObject(GameObject* obj)
{
	auto ent = registry.create();
	objects[ent] = obj;
	return ent;
}

Scene::Scene()
{
}
