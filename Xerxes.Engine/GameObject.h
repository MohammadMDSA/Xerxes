#pragma once

#include "Transform.h"
#include <memory>

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/ImGuizmo.h"

class GameObject
{
public:
	GameObject();

	Transform transform;

	void					OnStart(ID3D11Device1* device, ID3D11DeviceContext1* context);
	void					OnAwake();
	void					OnUpdate();
	void					OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, ID3D11DeviceContext1* context);
	void					OnDestroy();

	void					OnGizmo();
	void					OnInspector();


private:

	std::unique_ptr<DirectX::GeometricPrimitive>    m_shape;


	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::IEffectFactory> m_fxFactory;
	std::unique_ptr<DirectX::GeometricPrimitive> m_model;

	ImGuizmo::OPERATION			manipulationOperation;
	ImGuizmo::MODE				manipulationMode;
};

