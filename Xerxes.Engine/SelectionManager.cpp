#include "pch.h"
#include "SelectionManager.h"

void SelectionManager::OnInit()
{
	selectedGameObject = nullptr;
	selectedResource = nullptr;
	resourceSelected = false;
}

void SelectionManager::OnShutdown()
{
}

void SelectionManager::SetSelectedGameObject(GameObject* go)
{
	selectedGameObject = go;
	selectedResource = nullptr;
	resourceSelected = false;
}

void SelectionManager::SetSelectedResource(GameResourceBase* resource)
{
	selectedResource = resource;
	selectedGameObject = nullptr;
	resourceSelected = true;
}

bool SelectionManager::IsResourceSelected()
{
	return resourceSelected;
}

IInspectorDrawer* SelectionManager::GetSelectedInspectorDrawer()
{
	if (resourceSelected)
		return selectedResource;
	else
		return selectedGameObject;
}
