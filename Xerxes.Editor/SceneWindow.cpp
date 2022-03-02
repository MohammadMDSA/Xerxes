#include "pch.h"
#include "SceneWindow.h"
#include "Libs/imgui/imgui.h"
#include "RootManager.h"

SceneWindow::SceneWindow(int id) :
	EditorWindow(id, "Scene")
{
	
	backgroundAlpha = 0.f;
}

void SceneWindow::SetCamera(Camera* camera)
{
	this->camera = camera;
}

Camera* SceneWindow::GetCamera()
{
	return camera;
}

void SceneWindow::OnGUI()
{
}

int SceneWindow::GetCustomWindowFlags()
{
	return ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking;
}

void SceneWindow::Update(float deltaTime)
{
	if (!hasFocus)
		return;
	auto keyboard = RootManager::GetInstance()->GetInputManager()->GetKeyboard();
	auto kb = keyboard->GetState();
	if (kb.W && backgroundAlpha < 0.5)
	{
		backgroundAlpha == 1.f;

	}
}
