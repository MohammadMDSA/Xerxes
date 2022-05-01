#include "pch.h"
#include "ParticleSystemComponent.h"
#include "ParticleEffect.h"
#include "RootManager.h"
#include <boost/serialization/export.hpp>

using namespace Xerxes::Engine::Graphics::Effects;
using namespace boost::uuids;

XCOMP_GENERATE_DEFINITION(ParticleSystemComponent)

RTTR_REGISTRATION
{

}

ParticleSystemComponent::ParticleSystemComponent() :
	GameObjectComponent(),
	spawnDeviationSpace(ParticleSpace::Self)
{
	XCOMP_GENERATE_CONSTRUCTOR(ParticleSystemComponent)
}

ParticleSystemComponent::ParticleSystemComponent(const ParticleSystemComponent& other) :
	GameObjectComponent(other)
{
	this->effectId = other.effectId;
}

void ParticleSystemComponent::OnRender(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* context)
{
	auto effectRes = XResourceMEffect()::GetById(effectId);
	if (auto particleEffect = dynamic_cast<ParticleEffect*>(effectRes->GetResource()); particleEffect)
	{
		particleEffect->SetWorld(gameObject->transform().GetUnscaledWorld());
		particleEffect->SetView(view);
		particleEffect->SetProjection(proj);
	}
	particleSystem->Render(context, effectRes);
	if (XSelectionM()->GetSelectedInspectorDrawer() == this->gameObject)
		startBoundingBox->Draw(
			Matrix::CreateScale(particleSystem->particleDeviation) * gameObject->transform().GetUnscaledWorld(), view, proj, Colors::Gray, nullptr, true);
}

void ParticleSystemComponent::OnStart()
{
	auto resourceManager = XResourceM();
	particleSystem = std::make_unique<Xerxes::Engine::Graphics::ParticleSystem>();
	auto textureResource = resourceManager->ResourceGroup<TextureResource>::GetByName("star.dds");
	particleSystem->SetTextureId(textureResource->get_id());
	effectId = resourceManager->ResourceGroup<EffectResource>::GetByName("Particle Effect")->get_id();

	particleSystem->Initialize(resourceManager->GetDevice());

	startBoundingBox = DirectX::GeometricPrimitive::CreateBox(resourceManager->GetDeviceContext(), Vector3::One);
}

void ParticleSystemComponent::OnAwake()
{
}

void ParticleSystemComponent::OnUpdate(float deltaTime)
{
	auto root = RootManager::GetInstance();
	auto resourceManager = root->GetResourceManager();
	auto camera = root->GetCameraManager()->GetActiveCamera();
	particleSystem->Update(deltaTime, resourceManager->GetDeviceContext());

	auto effectRes = XResourceMEffect()::GetById(effectId);

	switch (spawnDeviationSpace)
	{
	case ParticleSpace::Local:
		particleSystem->particleDeviation = gameObject->transform().GetScale();
		break;
	case ParticleSpace::Hierarchy:
		particleSystem->particleDeviation = gameObject->transform().GetWorldScale();
		break;
	}
}

void ParticleSystemComponent::OnGizmo(ImGuizmo::OPERATION manipulationOperation, ImGuizmo::MODE manipulationMode)
{
}

void ParticleSystemComponent::OnInspector()
{
	auto availableWidth = ImGui::GetContentRegionAvail().x;

	// Particle spawn per second
	auto particlesPerSecond = particleSystem->GetparticlePerSecond();
	if (ImGui::DragFloat("Particles Per Second", &particlesPerSecond))
		particleSystem->SetparticlePerSecond(particlesPerSecond);

	ImGui::Spacing();

	// Particle lifetime
	ImGui::DragFloat("Life Time", &particleSystem->particleLifeTime, 0.01f);

	ImGui::Spacing();

	if (ImGui::CollapsingHeader("Velocity"))
	{
		ImGui::Indent();
		// Particle initial velocity
		ImGui::DragFloat3("Initial Velocity", (float*)&particleSystem->particleInitialVelocity, 0.1f);

		ImGui::DragFloat3("Initial Velocity Deviation", (float*)&particleSystem->particleInitialVelocityDeviation, 0.1f);
		ImGui::Unindent();
	}

	ImGui::Spacing();

	if (ImGui::CollapsingHeader("Spawn Bound"))
	{
		ImGui::Indent();

		// Particle spawn space scale type
		static int particleSpawnScaleSelected = 0;
		if (ImGui::Combo("Particle Spawn Scale Type", &particleSpawnScaleSelected, "Self\0Local\0Hierarchy\0\0"))
		{
			switch (particleSpawnScaleSelected)
			{
			case 0:
				spawnDeviationSpace = ParticleSpace::Self;
				break;
			case 1:
				spawnDeviationSpace = ParticleSpace::Local;
				break;
			default:
				spawnDeviationSpace = ParticleSpace::Hierarchy;
				break;
			}
		}

		// Particle self spawn space size
		if (spawnDeviationSpace == ParticleSpace::Self)
		{
			ImGui::DragFloat3("Particle Spawn Bound", (float*)&particleSystem->particleDeviation, 0.1f);
		}

		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader("Color"))
	{
		ImGui::Indent();

		ImGui::ColorEdit4("Initial Color", (float*)&particleSystem->particleInitialColor);

		ImGui::DragFloat4("Initial Color Deviation", (float*)&particleSystem->particleInitialColorDeviation, 0.01f, 0.f, 1.f);

		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader("Size"))
	{
		ImGui::Indent();

		ImGui::DragFloat2("Initial Size", (float*)&particleSystem->particleInitialSize, 0.01f);

		ImGui::DragFloat2("Initial Size Deviation", (float*)&particleSystem->particleInitialSizeDeviation, 0.01f);

		ImGui::Unindent();
	}

	// Particle texture
	{
		auto resourceManager = XResourceM();
		auto texResource = resourceManager->ResourceGroup<TextureResource>::GetById(particleSystem->textureReourceId);
		std::string resourceName = texResource ? texResource->GetName() : "[select texture]";

		ImGui::Spacing();

		if (ImGui::Button(resourceName.c_str(), ImVec2(0.6f * availableWidth, 0)))
			ImGui::OpenPopup("TextureSelection");

		ImGui::SameLine();
		ImGui::Text("Texture");

		if (ImGui::BeginPopup("TextureSelection"))
		{
			auto textures = resourceManager->ResourceGroup<TextureResource>::GetAll();
			if (ImGui::Selectable("<none>"))
			{
				particleSystem->textureReourceId = uuid();
			}

			int i = 0;
			for (auto it : textures)
			{
				ImGui::PushID(++i);
				if (ImGui::Selectable((it->GetName() + " (" + it->GetType() + ")").c_str()))
				{
					particleSystem->textureReourceId = it->get_id();
				}
				ImGui::PopID();
			}
			ImGui::EndPopup();
		}
		ImGui::Spacing();

	}
}

void ParticleSystemComponent::OnDestroy()
{
	particleSystem->Shutdown();
	particleSystem.release();
	startBoundingBox.release();
}

ParticleSystemComponent& ParticleSystemComponent::operator=(const ParticleSystemComponent& other)
{
	this->gameObject = other.gameObject;
	this->effectId = other.effectId;
	return *this;
}