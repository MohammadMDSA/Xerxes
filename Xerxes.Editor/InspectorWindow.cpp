#include "InspectorWindow.h"
#include "Libs/imgui/imgui.h"

using namespace Xerxes::Editor::Panels;

InspectorWindow::InspectorWindow(int id) : 
	EditorWindow(id, "Inspector")
{
	backgroundAlpha = 1.f;
	//SetDimansion(300.f, 900.f);
}

void InspectorWindow::OnGUI()
{
}

int InspectorWindow::GetCustomWindowFlags()
{
	//return ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking;
	return 0;
}

void InspectorWindow::Update(float deltaTime)
{
}
