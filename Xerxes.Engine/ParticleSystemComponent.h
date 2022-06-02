#pragma once
#include "GameResource.h"
#include "ParticleSystem.h"
#include "GameObjectComponent.h"
#include "boost/serialization/base_object.hpp"
#include "boost/serialization/access.hpp"

using namespace entt::literals;
using namespace Xerxes::Engine;

class ParticleSystemComponent : public GameObjectComponent
{
	XCOMP(ParticleSystemComponent)
public:
	ParticleSystemComponent();
	ParticleSystemComponent(const ParticleSystemComponent& other);
	~ParticleSystemComponent() = default;

	// Inherited via GameObjectComponent
	virtual void OnRender(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* context) override;
	virtual void OnStart() override;
	virtual void OnAwake() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnGizmo(ImGuizmo::OPERATION manipulationOperation, ImGuizmo::MODE manipulationMode) override;
	virtual void OnInspector() override;
	virtual void OnDestroy() override;

	ParticleSystemComponent& operator=(const ParticleSystemComponent& other);


private:
	friend class boost::serialization::access;

	enum ParticleSpace
	{
		Self,
		Local,
		Hierarchy
	};

	std::unique_ptr<Xerxes::Engine::Graphics::ParticleSystem>			particleSystem;
	GameResourceId effectId;

	std::unique_ptr<DirectX::GeometricPrimitive> startBoundingBox;
	ParticleSpace spawnDeviationSpace;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		boost::serialization::void_cast_register<ParticleSystemComponent, GameObjectComponent>(
			static_cast<ParticleSystemComponent*>(NULL),
			static_cast<GameObjectComponent*>(NULL)
			);
		ar& effectId;
	}

};
