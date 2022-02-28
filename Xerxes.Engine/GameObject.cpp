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
		if (ImGui::DragFloat3("Position", (float*)&(pos), 0.01f))
			transform.SetPositionV(pos);

		auto rotx = transform.GetRotationX();
		if (ImGui::DragFloat("Rotation X", &rotx, 0.1f))
		{
			transform.SetRotationX(rotx);
		}
		auto roty = transform.GetRotationY();
		if (ImGui::DragFloat("Rotation Y", &roty, 0.1f))
		{
			transform.SetRotationY(roty);
		}
		auto rotz = transform.GetRotationZ();
		if (ImGui::DragFloat("Rotation Z", &rotz, 0.1f))
		{
			transform.SetRotationZ(rotz);
		}

		auto scl = transform.GetScale();
		if (ImGui::DragFloat3("Scale", (float*)&(scl), 0.01f))
			transform.SetScaleV(scl);
	}

	OnInspector();
}

void GameObject::OnInspector()
{
}
