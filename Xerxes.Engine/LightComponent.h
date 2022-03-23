#pragma once
#include "GameObjectComponent.h"

using namespace entt::literals;

class LightComponent : public GameObjectComponent
{
	XCOMP_GENERATE_BODY()

public:
	LightComponent();
	~LightComponent();

	// Inherited via GameObjectComponent
	virtual void OnRender(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* context) override;
	virtual void OnStart() override;
	virtual void OnAwake() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnGizmo(ImGuizmo::OPERATION manipulationOperation, ImGuizmo::MODE manipulationMode) override;
	virtual void OnInspector() override;
	virtual void OnDestroy() override;
	virtual std::string GetName() override;

	void							SetColor(DirectX::SimpleMath::Vector3 color);
	const DirectX::SimpleMath::Vector3&	GetColor() const;
	const DirectX::SimpleMath::Vector3&	GetDirection() const;

private:
	DirectX::SimpleMath::Vector3	color;

};

