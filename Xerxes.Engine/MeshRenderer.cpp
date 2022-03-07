#include "pch.h"
#include "MeshRenderer.h"
#include <string>
#include "RootManager.h"

using namespace DirectX;
using namespace std;

MeshRenderer::MeshRenderer() :
	modelResourceId(-1),
	resource(nullptr)
{
}

void MeshRenderer::OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, ID3D11DeviceContext* context)
{
	if (resource)
		resource->GetResource()->Draw(context, *m_states, gameObject->transform.GetWorldMatrix(), view, proj);
}

void MeshRenderer::OnStart()
{
	auto resourceManager = RootManager::GetInstance()->GetResourceManager();
	m_states = std::make_unique<CommonStates>(resourceManager->GetDevice());
}

void MeshRenderer::OnAwake()
{
}

void MeshRenderer::OnUpdate(float deltaTime)
{
}

void MeshRenderer::OnGizmo()
{
}

void MeshRenderer::OnInspector()
{
	std::string resourceName = resource ? resource->GetName() : "[select model]";
	ImGui::Text("Model: ");
	ImGui::SameLine();
	if (ImGui::Button(resourceName.c_str()))
		ImGui::OpenPopup("MeshRendererModelSelection");

	if (ImGui::BeginPopup("MeshRendererModelSelection"))
	{
		ImGui::Text("Models");
		auto models = RootManager::GetInstance()->GetResourceManager()->GetAllModels();
		ImGui::Separator();
		if (ImGui::Selectable("<none>"))
		{
			modelResourceId = -1;
			resource = nullptr;
		}

		int i = 0;
		for (auto it : models)
		{
			ImGui::PushID(++i);
			if (ImGui::Selectable(it->GetName().c_str()))
			{
				modelResourceId = it->GetId();
				resource = it;
			}
			ImGui::PopID();
		}
		ImGui::EndPopup();
	}
}

void MeshRenderer::OnDestroy()
{
}

std::string MeshRenderer::GetName()
{
	return "Mesh Renderer";
}

void MeshRenderer::SetModelResourceId(int id)
{
	this->modelResourceId = id;

	resource = RootManager::GetInstance()->GetResourceManager()->GetModel(modelResourceId);
}
