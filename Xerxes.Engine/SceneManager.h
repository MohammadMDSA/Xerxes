#pragma once
#include "IManager.h"
#include "GameObject.h"

class SceneManager : public IManager
{
public:
	SceneManager();
	~SceneManager();

	void		Update(float deltaTime);
	void		OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void		OnGizmo();
	void		OnInspector();

	GameObject* GetSelectedGameObject();
	void		SetSelectedGameObject(GameObject* go);

	// Inherited via IManager
	virtual void OnInit() override;
	virtual void OnShutdown() override;

	std::vector<GameObject*>*			GetGameObjects();
	void								AddGameObject(GameObject* obj);
	void								RemoveGameObject(GameObject* obj);

private:

	GameObject*							selectedGameObject;
	std::vector<GameObject*>			gameObjects;

};

