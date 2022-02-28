#pragma once
#include "EditorWindow.h"

class InspectorWindow : public EditorWindow
{
public:
	InspectorWindow(int id);



	// Inherited via EditorWindow
	virtual void OnGUI() override;
	virtual int GetCustomWindowFlags() override;

};

