#include "pch.h"
#include "GameObject.h"
#include "RootManager.h"

using namespace DirectX;

GameObject::GameObject() :
	manipulationOperation(ImGuizmo::OPERATION::TRANSLATE)
{
}

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
	auto camera = RootManager::GetInstance()->GetCameraManager()->GetActiveCamera();
	auto view = camera->GetView();
	auto projection = camera->GetProjection();
	ImGuizmo::SetDrawlist();
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y); 
	auto world = transform.GetWorldMatrix();
	if (ImGuizmo::Manipulate((float*)&view, (float*)&projection, manipulationOperation, ImGuizmo::MODE::LOCAL, (float*)&(world)))
	{
		transform.SetWorld(world);
	}
}

void GameObject::OnInspector()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{

		ImGui::RadioButton("Position", (int*)&manipulationOperation, ImGuizmo::OPERATION::TRANSLATE);
		ImGui::SameLine();
		ImGui::RadioButton("Rotation", (int*)&manipulationOperation, ImGuizmo::OPERATION::ROTATE);
		ImGui::SameLine();
		ImGui::RadioButton("Scale", (int*)&manipulationOperation, ImGuizmo::OPERATION::SCALE);

		auto pos = transform.GetPosition();
		if (ImGui::DragFloat3("Position", (float*)&(pos), 0.01f))
			transform.SetPositionV(pos);

		float rotations[3] = { transform.GetRotationX(), transform.GetRotationY(), transform.GetRotationZ() };
	
		if (ImGui::DragFloat3("Rotation", (float*)&rotations, 0.1f))
		{
			transform.SetRotationX(rotations[0]);
			transform.SetRotationY(rotations[1]);
			transform.SetRotationZ(rotations[2]);
		}

		auto scl = transform.GetScale();
		if (ImGui::DragFloat3("Scale", (float*)&(scl), 0.01f))
			transform.SetScaleV(scl);
	}
}