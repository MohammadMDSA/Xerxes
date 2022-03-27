#include "pch.h"
#include "ParticleSystemComponent.h"
#include "ParticleEffect.h"
#include "RootManager.h"

using namespace Xerxes::Engine::Graphics::Effects;

XCOMP_GENERATE_DEFINITION(ParticleSystemComponent)

ParticleSystemComponent::ParticleSystemComponent() :
	GameObjectComponent(XNameOf(ParticleSystemComponent))
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
	auto effectRes = ResourceMEffect()::GetById(effectId);
	if (auto particleEffect = dynamic_cast<ParticleEffect*>(effectRes->GetResource()); particleEffect)
	{
		particleEffect->SetView(view);
		particleEffect->SetProjection(proj);
	}
	particleSystem->Render(context, effectRes);
}

void ParticleSystemComponent::OnStart()
{
	auto resourceManager = ResourceM();
	particleSystem = std::make_unique<Xerxes::Engine::Graphics::ParticleSystem>();
	auto textureResource = resourceManager->ResourceGroup<TextureResource>::GetByName("pants_norm.dds");
	particleSystem->SetTextureId(textureResource->GetId());
	effectId = resourceManager->ResourceGroup<EffectResource>::GetByName("Particle Effect")->GetId();

	particleSystem->Initialize(resourceManager->GetDevice());
}

void ParticleSystemComponent::OnAwake()
{
}

void ParticleSystemComponent::OnUpdate(float deltaTime)
{
	auto root = RootManager::GetInstance();
	auto resourceManager = root->GetResourceManager();
	auto camera = root->GetCameraManager()->GetActiveCamera();
	particleSystem->Update(deltaTime, resourceManager->GetDeviceContext(), camera->GetView());

	auto effectRes = ResourceMEffect()::GetById(effectId);
	if (auto particleEffect = dynamic_cast<ParticleEffect*>(effectRes->GetResource()); particleEffect)
	{
		particleEffect->SetWorld(gameObject->transform().GetWorldMatrix());
	}
}

void ParticleSystemComponent::OnGizmo(ImGuizmo::OPERATION manipulationOperation, ImGuizmo::MODE manipulationMode)
{
}

void ParticleSystemComponent::OnInspector()
{
}

void ParticleSystemComponent::OnDestroy()
{
	particleSystem->Shutdown();
	particleSystem.release();
}

ParticleSystemComponent& ParticleSystemComponent::operator=(const ParticleSystemComponent& other)
{
	this->name = other.name;
	this->gameObject = other.gameObject;
	this->effectId = other.effectId;
	return *this;
}