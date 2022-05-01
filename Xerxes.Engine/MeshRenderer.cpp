#include "pch.h"
#include "MeshRenderer.h"
#include <string>
#include "RootManager.h"
#include "GeometricPrimitiveResource.h"
#include <boost/serialization/export.hpp>

using namespace DirectX;
using namespace std;
using namespace boost::uuids;

XCOMP_GENERATE_DEFINITION(MeshRenderer)
RTTR_REGISTRATION
{
	XRegisterClassCns(MeshRenderer)
		XRegisterProperty(meshResourceId, MeshRenderer)
		XRegisterProperty(usingPrimitives, MeshRenderer)
		XRegisterProperty(effectResourceId, MeshRenderer);
}

MeshRenderer::MeshRenderer() :
	GameObjectComponent(),
	meshResourceId(uuid()),
	usingPrimitives(true),
	effectResourceId(uuid())
{
	XCOMP_GENERATE_CONSTRUCTOR(MeshRenderer)
}

MeshRenderer::MeshRenderer(const MeshRenderer& other) :
	GameObjectComponent(other)
{
	this->meshResourceId = other.meshResourceId;
	this->usingPrimitives = other.usingPrimitives;
	this->effectResourceId = other.effectResourceId;
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
		resource->GetResource()->Draw(context, *states, world, view, proj);
	}
}

void MeshRenderer::OnStart()
{
	auto resourceManager = RootManager::GetInstance()->GetResourceManager();
	states = std::make_unique<CommonStates>(resourceManager->GetDevice());
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
			meshResourceId = uuid();
		}

		int i = 0;
		for (auto it : models)
		{
			ImGui::PushID(++i);
			if (ImGui::Selectable((it->GetName() + " (" + it->GetType() + ")").c_str()))
			{
				meshResourceId = it->get_id();
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
				meshResourceId = it->get_id();
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
			effectResourceId = uuid();
		}

		int i = 0;
		for (auto it : effects)
		{
			ImGui::PushID(++i);
			if (ImGui::Selectable((it->GetName() + " (" + it->GetType() + ")").c_str()))
			{
				effectResourceId = it->get_id();
			}
			ImGui::PopID();
		}
		ImGui::EndPopup();
	}
}

void MeshRenderer::OnDestroy()
{
	states.release();
}

void MeshRenderer::SetModelResourceId(GameResourceId id)
{
	this->meshResourceId = id;
}

MeshRenderer& MeshRenderer::operator=(const MeshRenderer& other)
{
	this->gameObject = other.gameObject;
	this->meshResourceId = other.meshResourceId;
	this->usingPrimitives = other.usingPrimitives;
	this->effectResourceId = other.effectResourceId;
	return *this;
}

void MeshRenderer::RenderPrimitive()
{
}

void MeshRenderer::RenderModel()
{
}
