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
	void			OnGizmo(ImGuizmo::OPERATION manipulationOperation, ImGuizmo::MODE manipulationMode);

	// Inherited via IManager
	virtual void	OnInit() override;
	virtual void	OnShutdown() override;

	Scene*								GetCurrentScene();
	void								SetCurrentScene(Scene* scene);

private:

	std::vector<GameObject*>			gameObjects;
	Scene*								scene;
};

