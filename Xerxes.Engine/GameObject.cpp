#include "pch.h"
#include "GameObject.h"
#include "Libs/imgui/imgui.h"

void GameObject::OnStart(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context)
{
	m_shape = DirectX::GeometricPrimitive::CreateCube(context.Get());

}

void GameObject::OnAwake()
{
}

void GameObject::OnUpdate()
{

}

void GameObject::OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_shape->Draw(transform.GetWorldMatrix(), view, proj);
}

void GameObject::OnDestroy()
{
}

void GameObject::OnGizmo()
{
}

void GameObject::OnInspectorBase()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{

		auto pos = transform.GetPosition();
		if (ImGui::DragFloat3("Position", (float*)&(pos), 0.1f))
			transform.SetPositionV(pos);

		auto rot = transform.GetRotation();
		if (ImGui::DragFloat3("Rotation", (float*)&(rot), 0.1f))
			transform.SetRotationV(rot);

		auto scl = transform.GetScale();
		if (ImGui::DragFloat3("Scale", (float*)&(scl), 0.1f))
			transform.SetScaleV(scl);
	}

	OnInspector();
}

void GameObject::OnInspector()
{
}
