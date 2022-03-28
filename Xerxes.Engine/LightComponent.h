#pragma once
#include "GameObjectComponent.h"
#include "boost/serialization/access.hpp"

using namespace entt::literals;

class LightComponent : public GameObjectComponent
{
	XCOMP_GENERATE_BODY()

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

	const DirectX::SimpleMath::Vector3&	GetColor() const;
	const DirectX::SimpleMath::Vector3&	GetDirection() const;
	const float& GetAmbientIntencity() const;

	void SetColor(DirectX::SimpleMath::Vector3 color);
	void SetAmbientIntencity(float intencity);

private:
	friend class boost::serialization::access;

	DirectX::SimpleMath::Vector3	color;
	float							ambientIntencity;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& boost::serialization::base_object<GameObjectComponent>(*this);
		ar& color.x;
		ar& color.y;
		ar& color.z;
		ar& ambientIntencity;
	}

};

