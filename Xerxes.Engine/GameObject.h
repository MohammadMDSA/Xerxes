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

	void					OnStart(ID3D11Device1* device, ID3D11DeviceContext1* context);
	void					OnAwake();
	void					OnUpdate(float deltaTime);
	void					OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, ID3D11DeviceContext1* context);
	void					OnDestroy();

	void					OnGizmo();
	void					OnInspector();

	void					AddComponent(GameObjectComponent* component);
	void					DeleteComponent(GameObjectComponent* component);

private:

	std::vector<std::shared_ptr<GameObjectComponent>>	components;

	std::unique_ptr<DirectX::GeometricPrimitive>    m_shape;

	ImGuizmo::OPERATION			manipulationOperation;
	ImGuizmo::MODE				manipulationMode;
};

