#include "pch.h"
#include "MeshRenderer.h"
#include <string>
#include "RootManager.h"

using namespace DirectX;
using namespace std;

MeshRenderer::MeshRenderer() :
	meshResourceId(-1),
	usingPrimitives(true)
{
}

void MeshRenderer::OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, ID3D11DeviceContext* context)
{
	auto resourceManager = RootManager::GetInstance()->GetResourceManager();
	if (usingPrimitives)
	{
		//auto resource = resourceManager->
	}
	else
	{
		auto resource = resourceManager->GetModel(meshResourceId);
		if (!resource)
			return;
		resource->GetResource()->Draw(context, *m_states, gameObject->transform.GetWorldMatrix(), view, proj);
	}
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
	auto resourceManager = RootManager::GetInstance()->GetResourceManager();
	GameResourceBase* resource;
	if (usingPrimitives)
		resource = resourceManager->GetPrimitive(meshResourceId);
	else
		resource = resourceManager->GetModel(meshResourceId);

	std::string resourceName = resource ? resource->GetName() : "[select model]";
	ImGui::Text("Model: ");
	ImGui::SameLine();
	if (ImGui::Button(resourceName.c_str()))
		ImGui::OpenPopup("MeshRendererModelSelection");

	if (ImGui::BeginPopup("MeshRendererModelSelection"))
	{
		auto models = resourceManager->GetAllModels();
		if (ImGui::Selectable("<none>"))
		{
			meshResourceId = -1;
		}

		int i = 0;
		for (auto it : models)
		{
			ImGui::PushID(++i);
			if (ImGui::Selectable((it->GetName() + " (" + it->GetType() + ")").c_str()))
			{
				meshResourceId = it->GetId();
				usingPrimitives = false;
			}
			ImGui::PopID();
		}
		auto primitives = resourceManager->GetAllGeometricPrimitives();
		for (auto it : primitives)
		{
			ImGui::PushID(++i);
			if (ImGui::Selectable((it->GetName() + " (" + it->GetType() + ")").c_str()))
			{
				meshResourceId = it->GetId();
				usingPrimitives = true;
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
	this->meshResourceId = id;
}
