#include "pch.h"
#include "HierarchyWindow.h"
#include "RootManager.h"

HierarchyWindow::HierarchyWindow(int id)
    : EditorWindow(id, "Hierarchy")
{
    backgroundAlpha = 0.7f;
}

HierarchyWindow::~HierarchyWindow()
{
}

void HierarchyWindow::Update(float deltaTime)
{
}

void HierarchyWindow::OnGUI()
{
    auto sceneManager = RootManager::GetInstance()->GetSceneManager();
    auto selected = sceneManager->GetSelectedGameObject();
    
    int i = 0;
    for (auto go : *sceneManager->GetGameObjects())
    {
        ImGui::PushID(++i);
        if (ImGui::Selectable(go->GetName().c_str(), selected == go))
        {
            selected = go;
            sceneManager->SetSelectedGameObject(go);
        }
        ImGui::PopID();
    }
}

int HierarchyWindow::GetCustomWindowFlags()
{
    return 0;
}
