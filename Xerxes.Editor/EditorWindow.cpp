#include "EditorWindow.h"
#include "Libs/imgui/imgui.h"

void EditorWindow::SetFullscreen(bool fullscreen)
{
	this->fullscreen = fullscreen;
}

EditorWindow::EditorWindow(int id, std::string title) :
	firstFrame(true)
{
	this->id = id;
	this->title = title;
}

void EditorWindow::BeginWindow()
{
	ImGuiWindowFlags flags = 0;

	if (alwaysFullscreen) {
		flags |= GetForcedFullScreenFlags();
	}
	if (alwaysFullscreen || fullscreen)
	{
		flags |= GetFullScreenFlags();
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
	}
	flags |= GetCustomWindowFlags();
	ImGui::SetNextWindowBgAlpha(backgroundAlpha);
	ImGui::Begin(title.c_str(), &isOpen, flags);

	if (firstFrame)
		firstFrame = false;
	else
	{
		auto pos = ImGui::GetWindowPos();
		positionX = pos.x;
		positionY = pos.y;
		width = ImGui::GetWindowWidth();
		height = ImGui::GetWindowHeight();
	}
	OnGUI();
}

void EditorWindow::EndWindow()
{
	ImGui::End();
}
int EditorWindow::GetForcedFullScreenFlags()
{
	int flags = ImGuiWindowFlags_NoCollapse;
	return flags;
}

int EditorWindow::GetFullScreenFlags()
{
	int flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
	return flags;
}
