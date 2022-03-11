#pragma once

#include "EditorWindow.h"
#include "GameObject.h"
#include "SelectionManager.h"

class HierarchyWindow : public EditorWindow
{
public:
	HierarchyWindow(int id);
	~HierarchyWindow();

	// Inherited via EditorWindow
	virtual void Update(float deltaTime) override;
	virtual void OnGUI() override;

protected:
	virtual int GetCustomWindowFlags() override;

private:
	void DrawNodesHierarchy(std::vector<GameObject*>* objects, SelectionManager* selection);
	bool IsGameObjectInTargetHierarchy(GameObject* obj, Transform* target);
};

