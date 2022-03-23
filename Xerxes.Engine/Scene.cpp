#include "pch.h"
#include "Scene.h"

using namespace entt;

Scene::Scene() :
	name("New Scene")
{
}

Scene::~Scene()
{
	if (true)
	{

	}
}

std::vector<GameObject*> Scene::GetGameObjects()
{
	std::vector<GameObject*> result;
	registry.each([&result, this](entity ent)
		{
			result.push_back(objects[ent].get());
		});
	return result;
}

GameObject* Scene::CreateGameObject()
{
	auto entity = registry.create();

	objects[entity] = std::unique_ptr<GameObject>(new GameObject(this));
	auto go = objects[entity].get();
	go->entityId = entity;
	registry.emplace<Transform>(entity, go);
	return go;
}

GameObject* Scene::Instantiate(const GameObject& other)
{
	auto ent = registry.create();
	objects[ent] = std::unique_ptr<GameObject>(new GameObject(other));
	auto go = objects[ent].get();
	go->entityId = ent;
	for (auto&& curr : registry.storage()) {
		if (auto& storage = curr.second; storage.contains(other.entityId)) {
			storage.emplace(ent, storage.get(other.entityId));
		}
	}
	return go;
}

void Scene::RemoveGameObject(GameObject* gameObejct)
{
	auto ent = gameObejct->entityId;
	objects[ent].release();
	objects.erase(ent);
	registry.destroy(ent);
}

//template<class Archive>
//void Scene::save(Archive& ar, const unsigned int version) const


//template<class Archive>
//void Scene::load(Archive& ar, const unsigned int version)
