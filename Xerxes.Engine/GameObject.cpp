#include "pch.h"
#include "GameObject.h"
#include "Libs/imgui/imgui.h"

using namespace DirectX;

void GameObject::OnStart(ID3D11Device1* device, ID3D11DeviceContext1* context)
{
	m_states = std::make_unique<CommonStates>(device);

	m_fxFactory = std::make_unique<EffectFactory>(device);

	m_shape = DirectX::GeometricPrimitive::CreateCube(context);

}

void GameObject::OnAwake()
{
}

void GameObject::OnUpdate()
{

}

void GameObject::OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, ID3D11DeviceContext1* context)
{
	//m_shape->Draw(context, *m_states, transform.GetWorldMatrix(), view, proj);
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
