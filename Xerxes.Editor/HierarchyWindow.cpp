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
    
    for (auto go : *sceneManager->GetGameObjects())
    {
        if (ImGui::Selectable(go->GetName().c_str(), selected == go))
        {
            selected = go;
            sceneManager->SetSelectedGameObject(go);
        }
    }
}

int HierarchyWindow::GetCustomWindowFlags()
{
    return 0;
}
