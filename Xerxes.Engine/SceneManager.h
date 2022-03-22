#pragma once
#include "IManager.h"
#include "GameObject.h"
#include "Scene.h"

class SceneManager : public IManager
{
public:
	SceneManager();
	~SceneManager();

	void			Update(float deltaTime);
	void			OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void			OnGizmo();

	// Inherited via IManager
	virtual void	OnInit() override;
	virtual void	OnShutdown() override;

	Scene*								GetCurrentScene();

private:

	std::vector<GameObject*>			gameObjects;
	Scene*								scene;
};

