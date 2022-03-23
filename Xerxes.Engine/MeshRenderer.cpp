#include "pch.h"
#include "MeshRenderer.h"
#include <string>
#include "RootManager.h"
#include "GeometricPrimitiveResource.h"

using namespace DirectX;
using namespace std;

XCOMP_GENERATE_DEFINITION(MeshRenderer)

MeshRenderer::MeshRenderer() :
	meshResourceId(-1),
	usingPrimitives(true),
	effectResourceId(-1)
{
	XCOMP_GENERATE_CONSTRUCTOR(MeshRenderer)
}

void MeshRenderer::OnRender(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* context)
{
	auto resourceManager = RootManager::GetInstance()->GetResourceManager();
	auto world = gameObject->transform().GetWorldMatrix();
	auto lightManager = RootManager::GetInstance()->GetLightManager();
	if (usingPrimitives)
	{
		auto effect = resourceManager->ResourceGroup<EffectResource>::GetById(effectResourceId);
		auto resource = resourceManager->ResourceGroup<GeometricPrimitiveResource>::GetById(meshResourceId);
		if (!resource)
			return;
		if (effect)
		{
			auto effectRes = effect->GetResource();

			// Updating matrix effects
			auto effectMat = dynamic_cast<IEffectMatrices*>(effectRes);
			if (effectMat)
				effectMat->SetMatrices(world, view, proj);

			// Updating light effect
			auto effectLight = dynamic_cast<IEffectLights*>(effectRes);
			if (effectLight)
				lightManager->ApplyToEffect(effectLight);

			resource->GetResource()->Draw(effectRes, effect->GetInputLayout());
		}
		else
			resource->GetResource()->Draw(world, view, proj);
	}
	else
	{
		auto resource = resourceManager->ResourceGroup<ModelResource>::GetById(meshResourceId);
		if (!resource)
			return;
		resource->GetResource()->UpdateEffects([lightManager, world, view, proj](IEffect* effect)
			{
				auto lightEffect = dynamic_cast<IEffectLights*>(effect);
				if (lightEffect)
				{
					lightManager->ApplyToEffect(lightEffect);
				}

				auto matrixEffect = dynamic_cast<IEffectMatrices*>(effect);
				if (matrixEffect)
				{
					matrixEffect->SetMatrices(world, view, proj);
				}

			});
		resource->GetResource()->Draw(context, *m_states, world, view, proj);
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

void MeshRenderer::OnGizmo(ImGuizmo::OPERATION manipulationOperation, ImGuizmo::MODE manipulationMode)
{
}

void MeshRenderer::OnInspector()
{
	auto resourceManager = RootManager::GetInstance()->GetResourceManager();
	GameResourceBase* resource;

	auto effectResource = resourceManager->ResourceGroup<EffectResource>::GetById(effectResourceId);

	if (usingPrimitives)
		resource = resourceManager->ResourceGroup<GeometricPrimitiveResource>::GetById(meshResourceId);
	else
		resource = resourceManager->ResourceGroup<ModelResource>::GetById(meshResourceId);

	std::string resourceName = resource ? resource->GetName() : "[select model]";
	std::string effectName = effectResource ? effectResource->GetName() : "[select effect]";
	ImGui::Text("Model: ");
	ImGui::SameLine();
	if (ImGui::Button(resourceName.c_str()))
		ImGui::OpenPopup("MeshRendererModelSelection");

	if (ImGui::BeginPopup("MeshRendererModelSelection"))
	{
		auto models = resourceManager->ResourceGroup<ModelResource>::GetAll();
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
		auto primitives = resourceManager->ResourceGroup<GeometricPrimitiveResource>::GetAll();
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

	ImGui::Spacing();
	ImGui::Text("Effect: ");
	ImGui::SameLine();
	if (ImGui::Button(effectName.c_str()))
		ImGui::OpenPopup("MeshRendererEffectSelection");

	if (ImGui::BeginPopup("MeshRendererEffectSelection"))
	{
		auto effects = resourceManager->ResourceGroup<EffectResource>::GetAll();
		if (ImGui::Selectable("<none>"))
		{
			effectResourceId = -1;
		}

		int i = 0;
		for (auto it : effects)
		{
			ImGui::PushID(++i);
			if (ImGui::Selectable((it->GetName() + " (" + it->GetType() + ")").c_str()))
			{
				effectResourceId = it->GetId();
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

void MeshRenderer::RenderPrimitive()
{
}

void MeshRenderer::RenderModel()
{
}
