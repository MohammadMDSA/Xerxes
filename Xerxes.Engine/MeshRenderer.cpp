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
		resource->resource->Draw(context, *m_states, gameObject->transform.GetWorldMatrix(), view, proj);
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
	std::string resourceName = resource ? resource->name : "[select model]";
	ImGui::Text("Model: ");
	ImGui::SameLine();
	if (ImGui::Button(resourceName.c_str()))
		ImGui::OpenPopup("MeshRendererModelSelection");

	if (ImGui::BeginPopup("MeshRendererModelSelection"))
	{
		ImGui::Text("Models");
		auto models = RootManager::GetInstance()->GetResourceManager()->GetAllModels();
		ImGui::Separator();
		for (auto it : models)
		{
			if (ImGui::Selectable(it->name.c_str()))
			{
				modelResourceId = it->id;
				resource = it;
			}
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
