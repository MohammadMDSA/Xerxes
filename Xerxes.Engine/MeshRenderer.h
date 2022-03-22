#pragma once
#include "GameObjectComponent.h"
#include "ResourceManager.h"
#include <string>

class MeshRenderer : public GameObjectComponent
{
	XCOMP_GENERATE_BODY()
public:
	MeshRenderer();

	// Inherited via GameObjectComponent
	virtual void OnRender(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* context) override;
	virtual void OnStart() override;
	virtual void OnAwake() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnGizmo() override;
	virtual void OnInspector() override;
	virtual void OnDestroy() override;
	virtual std::string GetName() override;

	void SetModelResourceId(int id);
private:

	std::unique_ptr<DirectX::CommonStates> m_states;
	int meshResourceId;
	bool usingPrimitives;
	int effectResourceId;

	void RenderPrimitive();
	void RenderModel();
};

