#include "pch.h"
#include "GameObject.h"
#include "RootManager.h"
#include <filesystem>
#include <PathCch.h>
#include "Libs/imgui/imgui_stdlib.h"
#include "MeshRenderer.h"

using namespace DirectX;
using namespace std;

GameObject::GameObject() :
	manipulationOperation(ImGuizmo::OPERATION::TRANSLATE),
	manipulationMode(ImGuizmo::MODE::LOCAL),
	name("GameObject"),
	editName("GameObject"),
	isAwake(false),
	isStarted(false)
{
}

void GameObject::OnStart()
{
	if (isStarted)
		return;
	isStarted = true;

	for (auto component : components)
	{
		component->OnStart();
	}
}

void GameObject::OnAwake()
{
	if (isAwake)
		return;
	isAwake = true;
	for (auto component : components)
	{
		component->OnAwake();
	}
}

void GameObject::OnUpdate(float deltaTime)
{
	for (auto component : components)
	{
		component->OnUpdate(deltaTime);
	}
}

void GameObject::OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, ID3D11DeviceContext* context)
{
	for (auto component : components)
	{
		component->OnRender(view, proj, context);
	}
}

void GameObject::OnDestroy()
{
	for (auto component : components)
	{
		component->OnDestroy();
	}

}

void GameObject::OnGizmo()
{
	auto camera = RootManager::GetInstance()->GetCameraManager()->GetActiveCamera();
	auto view = camera->GetView();
	auto projection = camera->GetProjection();
	ImGuizmo::SetDrawlist();
	auto world = transform.GetWorldMatrix();
	if (ImGuizmo::Manipulate((float*)&view, (float*)&projection, manipulationOperation, manipulationMode, (float*)&(world)))
	{
		transform.SetWorld(world);
	}
	for (auto component : components)
	{
		component->OnGizmo();
	}
}

void GameObject::OnInspector()
{
	// GameObject name
	if (ImGui::InputText("Name", &editName, ImGuiInputTextFlags_CharsNoBlank))
	{
		SetName(editName);
	}

	if (!ImGui::IsItemActive() && editName.empty())
	{
		editName = std::string(name);
	}

	// GameObject Transformation
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{

		ImGui::Text("Manipulation Mode");
		ImGui::RadioButton("World", (int*)&manipulationMode, ImGuizmo::MODE::WORLD);
		ImGui::SameLine();
		ImGui::RadioButton("Local", (int*)&manipulationMode, ImGuizmo::MODE::LOCAL);

		ImGui::Separator();

		// Manipulation operation selection
		ImGui::Text("Manipulation Operation");
		ImGui::RadioButton("Position", (int*)&manipulationOperation, ImGuizmo::OPERATION::TRANSLATE);
		ImGui::SameLine();
		ImGui::RadioButton("Rotation", (int*)&manipulationOperation, ImGuizmo::OPERATION::ROTATE);
		ImGui::SameLine();
		ImGui::RadioButton("Scale", (int*)&manipulationOperation, ImGuizmo::OPERATION::SCALE);

		ImGui::Separator();

		// Transform properties
		ImGui::Text("Transform");
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

	// GameObject components
	for (auto component : components)
	{
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::CollapsingHeader(component->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			ImGui::Spacing();
			component->OnInspector();
	}

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	float width = 120.f;
	ImGui::SetNextItemWidth(width);
	ImGui::SetCursorPos(ImVec2((ImGui::GetWindowWidth() - width) / 2, ImGui::GetCursorPosY()));
	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponentSelection");

	if (ImGui::BeginPopup("AddComponentSelection"))
	{
		ImGui::Text("Components");
		ImGui::Separator();
		if (ImGui::Selectable("Mesh Renderer"))
		{
			this->AddComponent(new MeshRenderer());
		}
		ImGui::EndPopup();
	}
}

void GameObject::AddComponent(GameObjectComponent* component)
{
	for (auto comp : components)
	{
		if (comp.get() == component)
			return;
	}
	components.push_back(std::shared_ptr<GameObjectComponent>(component));
	component->gameObject = this;
	component->OnStart();
	if (isAwake)
		component->OnAwake();
}

void GameObject::DeleteComponent(GameObjectComponent* component)
{
	auto index = -1;
	for (int i = 0; i < components.size(); i++)
	{
		if (component == components.at(i).get())
		{
			index = i;
			break;
		}
	}
	if (index == -1)
		return;
	component->gameObject = nullptr;
	components.erase(components.begin() + index);
}

void GameObject::SetName(std::string name)
{
	if (!name.empty())
		this->name = name;
}

std::string GameObject::GetName()
{
	return this->name;
}
