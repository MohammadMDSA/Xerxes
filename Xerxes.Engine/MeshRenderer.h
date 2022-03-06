#pragma once
#include "GameObjectComponent.h"
#include "ResourceManager.h"
#include <string>

class MeshRenderer : public GameObjectComponent
{
public:
	MeshRenderer();

	// Inherited via GameObjectComponent
	virtual void OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, ID3D11DeviceContext1* context) override;
	virtual void OnStart(ID3D11Device1* device, ID3D11DeviceContext1* context) override;
	virtual void OnAwake() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnGizmo() override;
	virtual void OnInspector() override;
	virtual void OnDestroy() override;
	virtual std::string GetName() override;

	void SetModelResourceId(int id);

	PWSTR					address;
private:

	std::unique_ptr<DirectX::CommonStates> m_states;
	int modelResourceId;

	GameResource<DirectX::Model>* resource;

};
