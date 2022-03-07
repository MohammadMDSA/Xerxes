#pragma once
#include "EditorWindow.h"
#include "ResourceManager.h"

class ResourceWindow : public EditorWindow
{

public:
	ResourceWindow(int id);
	~ResourceWindow();


	// Inherited via EditorWindow
	virtual void Update(float deltaTime) override;

	virtual void OnGUI() override;

	virtual int GetCustomWindowFlags() override;

private:

	GameResourceBase* selectedResource;
};

