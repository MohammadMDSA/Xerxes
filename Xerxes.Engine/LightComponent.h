#pragma once
#include "GameObjectComponent.h"
#include "boost/serialization/access.hpp"
#include "Libs/rttr/inc/rttr/registration.h"
#include <rttr/type.h>
#include <rttr/property.h>
#include "TypeSerialization.h"

using namespace rttr;
using namespace entt::literals;

class LightComponent : public GameObjectComponent
{
	XCOMP(LightComponent)

public:
	LightComponent();
	LightComponent(const LightComponent& other);
	~LightComponent();

	// Inherited via GameObjectComponent
	virtual void OnRender(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* context) override;
	virtual void OnStart() override;
	virtual void OnAwake() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnGizmo(ImGuizmo::OPERATION manipulationOperation, ImGuizmo::MODE manipulationMode) override;
	virtual void OnInspector() override;
	virtual void OnDestroy() override;

	const DirectX::SimpleMath::Vector3&	GetDirection() const;

	LightComponent& operator=(const LightComponent& other);

private:
	friend class boost::serialization::access;
	
	XProperty(DirectX::SimpleMath::Vector3, color)
	XProperty(float, ambientIntencity)

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		boost::serialization::void_cast_register<LightComponent, GameObjectComponent>(
			static_cast<LightComponent*>(NULL),
			static_cast<GameObjectComponent*>(NULL)
			);

		ar& color;
		ar& ambientIntencity;
	}

};

