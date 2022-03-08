#include "pch.h"
#include "SceneManager.h"
#include "RootManager.h"

SceneManager::SceneManager()
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
	auto selection = RootManager::GetInstance()->GetSelectionManager();

	if (!selection->IsResourceSelected())
	{
		auto obj = dynamic_cast<GameObject*>(selection->GetSelectedInspectorDrawer());
		if (obj)
			obj->OnGizmo();
	}
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
