#include "pch.h"
#include "SceneManager.h"
#include "RootManager.h"
#include "Libs/EnTT/entt.hpp"

using namespace entt::literals;

SceneManager::SceneManager()
{
	scene = new Scene();
}

SceneManager::~SceneManager()
{
	delete scene;
}

void SceneManager::Start()
{
	for (auto obj : GetCurrentScene()->GetGameObjects())
		obj->OnAwake();
}

void SceneManager::Update(float deltaTime)
{

	for (auto obj : GetCurrentScene()->GetGameObjects())
		obj->OnUpdate(deltaTime);

}

void SceneManager::OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = RootManager::GetInstance()->GetResourceManager()->GetDeviceContext();
	for (auto gameObject : GetCurrentScene()->GetGameObjects())
	{
		gameObject->OnRender(view, proj, context);
	}
}

void SceneManager::OnGizmo(ImGuizmo::OPERATION manipulationOperation, ImGuizmo::MODE manipulationMode)
{
	auto selection = RootManager::GetInstance()->GetSelectionManager();

	if (!selection->IsResourceSelected())
	{
		auto obj = dynamic_cast<GameObject*>(selection->GetSelectedInspectorDrawer());
		if (obj)
			obj->OnGizmo(manipulationOperation, manipulationMode);
	}
}

void SceneManager::OnInit()
{
}

void SceneManager::OnShutdown()
{
}

Scene* SceneManager::GetCurrentScene()
{
	return scene;
}

void SceneManager::SetCurrentScene(Scene* scene)
{
	this->scene = scene;
}
