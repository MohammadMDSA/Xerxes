#include "pch.h"
#include "GameObject.h"
#include "RootManager.h"
#include <filesystem>
#include <PathCch.h>
#include "Libs/imgui/imgui_stdlib.h"
#include "MeshRenderer.h"

#include <rttr/type.h>

using namespace DirectX;
using namespace std;
using namespace entt::literals;

RTTR_REGISTRATION
{
	rttr::registration::class_<GameObject>(XNameOf(GameObject))
		.property("name", &GameObject::name)
		.property("isAwake", &GameObject::isAwake)
		.property("isStarted", &GameObject::isStarted)
		.property("destroyed", &GameObject::destroyed)
		.property("entityId", &GameObject::entityId)
		.property("scene", &GameObject::scene)
		.method("Create", &GameObject::Create);
}

GameObject::GameObject(Scene* scn) :
	name("GameObject"),
	editName("GameObject"),
	isAwake(false),
	isStarted(false),
	scene(scn),
	destroyed(false),
	entityId((entt::entity)(-1))
{
}

GameObject::GameObject(const GameObject& other)
{
	this->name = other.name;
	this->isAwake = other.isAwake;
	this->isStarted = other.isStarted;
}

void GameObject::SetTransform(const Transform& trans)
{
	auto& transform = scene->registry.emplace<Transform>(entityId, trans);
	transform.gameObjectId = entityId;
}

GameObject* GameObject::Create()
{
	return RootManager::GetInstance()->GetSceneManager()->GetCurrentScene()->CreateGameObject();
}

Transform& GameObject::transform() const
{
	return scene->registry.get<Transform>(entityId);
}

Transform& GameObject::transform()
{
	return scene->registry.get<Transform>(entityId);
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
	handle.visit([this](entt::id_type compTypeId, const auto& storage)
		{
			switch (compTypeId)
			{
			case entt::type_hash<LightComponent>():
				scene->registry.get<LightComponent>(entityId).OnDestroy();
				break;
			case entt::type_hash<MeshRenderer>():
				scene->registry.get<MeshRenderer>(entityId).OnDestroy();
				break;
			default:
				break;
			}
		});
}

void GameObject::OnGizmo(ImGuizmo::OPERATION manipulationOperation, ImGuizmo::MODE manipulationMode)
{
	auto camera = RootManager::GetInstance()->GetCameraManager()->GetActiveCamera();
	auto view = camera->GetView();
	auto projection = camera->GetProjection();
	ImGuizmo::SetDrawlist();
	auto world = transform().GetWorldMatrix();
	if (ImGuizmo::Manipulate((float*)&view, (float*)&projection, manipulationOperation, manipulationMode, (float*)&(world)))
	{
		transform().SetWorld(world);
	}
	for (auto component : GetComponents())
	{
		component->OnGizmo(manipulationOperation, manipulationMode);
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

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// GameObject Transformation
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Transform properties
		auto pos = transform().GetPosition();
		if (ImGui::DragFloat3("Position", (float*)&(pos), 0.01f))
			transform().SetPositionV(pos);

		float rotations[3] = { transform().GetRotationX(), transform().GetRotationY(), transform().GetRotationZ() };

		if (ImGui::DragFloat3("Rotation", (float*)&rotations, 0.1f))
		{
			transform().SetRotationX(rotations[0]);
			transform().SetRotationY(rotations[1]);
			transform().SetRotationZ(rotations[2]);
		}

		auto scl = transform().GetScale();
		if (ImGui::DragFloat3("Scale", (float*)&(scl), 0.01f))
			transform().SetScaleV(scl);
	}

	ImGuiStyle& style = ImGui::GetStyle();
	float originalIndentSpace = style.IndentSpacing;
	style.IndentSpacing = 10;

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
		{
			ImGui::Indent();
			ImGui::Spacing();
			component->OnInspector();
			ImGui::Unindent();
		}
	}
	style.IndentSpacing = originalIndentSpace;

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
				this->AttachComponent<MeshRenderer>();
			}
		if (!scene->registry.try_get<LightComponent>(entityId))
			if (ImGui::Selectable("Light"))
			{
				this->AttachComponent<LightComponent>();
			}
		if (!scene->registry.try_get<ParticleSystemComponent>(entityId))
			if (ImGui::Selectable("Particle System"))
			{
				this->AttachComponent<ParticleSystemComponent>();
			}
		ImGui::EndPopup();
	}
}
//
//template<class T>
//T& GameObject::AttachCopiedComponent(T* copyFrom)


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

template<typename T>
T& GameObject::GetComponent()
{
	return scene->registry.get<T>(entityId);
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

void GameObject::Destroy(GameObject* obj)
{
	auto scene = RootManager::GetInstance()->GetSceneManager()->GetCurrentScene();
	obj->destroyed = true;
	scene->RemoveGameObject(obj);
}

std::vector<GameObjectComponent*> GameObject::GetComponents() const
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
			case entt::type_hash<ParticleSystemComponent>():
				result.push_back(&scene->registry.get<ParticleSystemComponent>(entityId));
			default:
				break;
			}
		});
	return result;
}
