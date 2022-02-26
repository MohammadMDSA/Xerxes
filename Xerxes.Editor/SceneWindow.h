#pragma once
#include "EditorWindow.h"


class SceneWindow : public EditorWindow
{
public:
	SceneWindow(int id);
protected:

	// Inherited via EditorWindow
	virtual void OnGUI() override;

	// Inherited via EditorWindow
	virtual int GetCustomWindowFlags() override;
};

