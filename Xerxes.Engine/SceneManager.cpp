#include "pch.h"
#include "SceneManager.h"
#include "RootManager.h"

SceneManager::SceneManager() :
	selectedGameObject(nullptr)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update(float deltaTime)
{

	for (auto obj : gameObjects)
		obj->OnUpdate(deltaTime);

}

void SceneManager::OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = RootManager::GetInstance()->GetResourceManager()->GetDeviceContext();
	for (auto gameObject : gameObjects)
	{
		gameObject->OnRender(view, proj, context);
	}
}

void SceneManager::OnGizmo()
{
	if (selectedGameObject)
		selectedGameObject->OnGizmo();
}

void SceneManager::OnInspector()
{
	if (selectedGameObject)
		selectedGameObject->OnInspector();
}

GameObject* SceneManager::GetSelectedGameObject()
{
	return selectedGameObject;
}

void SceneManager::SetSelectedGameObject(GameObject* go)
{
	this->selectedGameObject = go;
}

void SceneManager::OnInit()
{
}

void SceneManager::OnShutdown()
{
}

std::vector<GameObject*>* SceneManager::GetGameObjects()
{
	return &gameObjects;
}

void SceneManager::AddGameObject(GameObject* obj)
{
	if (std::find(gameObjects.begin(), gameObjects.end(), obj) != gameObjects.end())
		return;
	this->gameObjects.push_back(obj);
}

void SceneManager::RemoveGameObject(GameObject* obj)
{
	std::remove(gameObjects.begin(), gameObjects.end(), obj);
}
