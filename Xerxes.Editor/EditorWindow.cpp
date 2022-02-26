#include "EditorWindow.h"
#include "EditorWindowManager.h"
#include "Libs/imgui/imgui.h"

EditorWindow::EditorWindow(int id, std::string title)
{
	this->id = id;
	this->title = title;
}

void EditorWindow::BeginWindow()
{
	ImGui::Begin(title.c_str());
}

void EditorWindow::EndWindow()
{
	ImGui::End();
}

void EditorWindow::DrawWindow()
{
	BeginWindow();
	OnGUI();
	EndWindow();
}

void EditorWindow::OnGUI()
{
	ImGui::Text("foooooooooooo");
}