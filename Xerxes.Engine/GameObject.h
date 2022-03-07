#pragma once

#include "Transform.h"
#include <memory>

#include "GameObjectComponent.h"

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/ImGuizmo.h"

class GameObjectComponent;

class GameObject
{
public:
	GameObject();

	Transform transform;

	void					OnStart();
	void					OnAwake();
	void					OnUpdate(float deltaTime);
	void					OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, ID3D11DeviceContext* context);
	void					OnDestroy();

	void					OnGizmo();
	void					OnInspector();

	void					AddComponent(GameObjectComponent* component);
	void					DeleteComponent(GameObjectComponent* component);

	void					SetName(std::string name);
	std::string				GetName();

private:

	std::vector<std::shared_ptr<GameObjectComponent>>	components;
	
	ImGuizmo::OPERATION			manipulationOperation;
	ImGuizmo::MODE				manipulationMode;
	std::string					name;
	std::string					editName;

	bool						isAwake;
	bool						isStarted;
};

