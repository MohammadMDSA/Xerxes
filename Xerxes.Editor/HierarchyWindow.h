#pragma once

#include "EditorWindow.h"

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
};

