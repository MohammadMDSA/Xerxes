#pragma once
#include "IManager.h"
#include "GameObject.h"
#include "GameResource.h"
#include "IInspectorDrawer.h"

class SelectionManager : public IManager
{

public:
	// Inherited via IManager
	virtual void OnInit() override;
	virtual void OnShutdown() override;

	void		SetSelectedGameObject(GameObject* go);
	void		SetSelectedResource(GameResourceBase* resource);
	bool		IsResourceSelected();
	IInspectorDrawer* GetSelectedInspectorDrawer();

private:

	GameObject*			selectedGameObject;
	GameResourceBase*	selectedResource;

	bool				resourceSelected;
};

