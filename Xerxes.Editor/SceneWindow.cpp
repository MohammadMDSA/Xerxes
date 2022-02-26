#include "SceneWindow.h"
#include "Libs/imgui/imgui.h"

SceneWindow::SceneWindow(int id) :
	EditorWindow(id, "Scene")
{
	
	backgroundAlpha = 0.f;
	alwaysFullscreen = true;
}

void SceneWindow::OnGUI()
{
}

int SceneWindow::GetCustomWindowFlags()
{
	return ImGuiWindowFlags_NoBringToFrontOnFocus || ImGuiWindowFlags_NoDocking;
}
