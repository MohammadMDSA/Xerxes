#include "pch.h"
#include "GameObject.h"
#include "RootManager.h"
#include <filesystem>
#include <PathCch.h>
#include "Libs/imgui/imgui_stdlib.h"
#include "MeshRenderer.h"

using namespace DirectX;
using namespace std;
using namespace entt::literals;

GameObject::GameObject(Scene* scn) :
	manipulationOperation(ImGuizmo::OPERATION::TRANSLATE),
	manipulationMode(ImGuizmo::MODE::LOCAL),
	name("GameObject"),
	editName("GameObject"),
	isAwake(false),
	isStarted(false),
	transform(Transform(this)),
	scene(scn)
{
	this->entityId = scn->AddGameObject(this);
}

GameObject::~GameObject()
{
	scene->registry.destroy(entityId);
}

void GameObject::OnStart()
{
	if (isStarted)
		return;
	isStarted = true;
	for (auto component : GetComponents())
	{
		component->OnStart();
	}
}

void GameObject::OnAwake()
{
	if (isAwake)
		return;
	isAwake = true;
	for (auto component : GetComponents())
	{
		component->OnAwake();
	}
}

void GameObject::OnUpdate(float deltaTime)
{
	for (auto component : GetComponents())
	{
		component->OnUpdate(deltaTime);
	}
}

void GameObject::OnRender(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* context)
{
	auto reg = &scene->registry;
	for (auto comp : GetComponents())
	{
		comp->OnRender(view, proj, context);
	}
}

void GameObject::OnDestroy()
{
	std::vector<GameObjectComponent*> result;
	auto reg = &scene->registry;
	auto handle = entt::handle(*reg, entityId);
	handle.visit([this, &result](entt::id_type compTypeId, const auto& storage)
		{
			switch (compTypeId)
			{
			case entt::type_hash<LightComponent>():
				result.push_back(&scene->registry.get<LightComponent>(entityId));
				break;
			case entt::type_hash<MeshRenderer>():
				result.push_back(&scene->registry.get<MeshRenderer>(entityId));
				break;
			default:
				break;
			}
		});
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
	for (auto component : GetComponents())
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
	auto components = GetComponents();
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
		if (!scene->registry.try_get<MeshRenderer>(entityId))
			if (ImGui::Selectable("Mesh Renderer"))
			{
				//this->AddComponent(new MeshRenderer());
				this->AttachComponent<MeshRenderer>();
			}
		if (!scene->registry.try_get<LightComponent>(entityId))
			if (ImGui::Selectable("Light"))
			{
				//this->AddComponent(new LightComponent());
				this->AttachComponent<LightComponent>();
			}
		ImGui::EndPopup();
	}
}

template<class T>
T& GameObject::AttachComponent()
{
	auto& comp = scene->registry.emplace<T>(entityId);
	auto& goComp = static_cast<GameObjectComponent&>(comp);
	goComp.gameObject = this;
	if (isStarted)
		goComp.OnStart();
	if (isAwake)
		goComp.OnAwake();
	return comp;
}

template<class T>
void GameObject::DeleteComponent()
{
	scene->registry.remove<T>(entityId);
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

std::vector<GameObjectComponent*> GameObject::GetComponents()
{
	std::vector<GameObjectComponent*> result;
	auto reg = &scene->registry;
	auto handle = entt::handle(*reg, entityId);
	handle.visit([this, &result](entt::id_type compTypeId, const auto& storage)
		{
			switch (compTypeId)
			{
			case entt::type_hash<LightComponent>():
				result.push_back(&scene->registry.get<LightComponent>(entityId));
				break;
			case entt::type_hash<MeshRenderer>():
				result.push_back(&scene->registry.get<MeshRenderer>(entityId));
				break;
			default:
				break;
			}
		});
	return result;
}
