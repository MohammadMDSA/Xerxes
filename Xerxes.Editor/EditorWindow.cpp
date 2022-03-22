#include "EditorWindow.h"
#include "Libs/imgui/imgui.h"

EditorWindow::EditorWindow(int id, std::string title) :
	firstFrame(true),
	alwaysFullscreen(false),
	fullscreen(false),
	hasFocus(false),
	shouldUpdateDimansion(false),
	shouldUpdatePosition(false),
	backgroundAlpha(0.8)
{
	this->id = id;
	this->title = title;
}

void EditorWindow::SetFullscreen(bool fullscreen)
{
	this->fullscreen = fullscreen;
}

void EditorWindow::SetPosition(float x, float y)
{
	//this->positionX = x;
	//this->positionY = y;
	//shouldUpdatePosition = true;
}

void EditorWindow::SetDimansion(float width, float height)
{
	this->width = width;
	this->height = height;
	shouldUpdateDimansion = true;
}

EditorWindow::~EditorWindow()
{
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
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
	}
	else
	{
		if (shouldUpdateDimansion)
		{
			ImGui::SetNextWindowSize(ImVec2(width, height));
			shouldUpdateDimansion = false;
		}
		if (shouldUpdatePosition)
		{
			ImGui::SetNextWindowPos(ImVec2(positionX, positionY));
			shouldUpdatePosition = false;
		}
	}

	flags |= GetCustomWindowFlags();
	ImGui::SetNextWindowBgAlpha(backgroundAlpha);

	OnGUIInit();

	ImGui::Begin(title.c_str(), &isOpen, flags);

	if (firstFrame)
		firstFrame = false;
	else
	{
		auto pos = ImGui::GetWindowPos();
		positionX = pos.x;
		positionY = pos.y;
		auto min = ImGui::GetWindowContentRegionMin();
		auto max = ImGui::GetWindowContentRegionMax();
		width = max.x - min.x;
		height = max.y - min.y;
		hasFocus = ImGui::IsWindowFocused();
		isHovered = ImGui::IsWindowHovered();
	}
}

void EditorWindow::EndWindow()
{
	OnGUI();
	ImGui::End();
}

int EditorWindow::GetForcedFullScreenFlags()
{
	int flags = ImGuiWindowFlags_NoCollapse;
	return flags;
}

int EditorWindow::GetFullScreenFlags()
{
	return ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;
}
