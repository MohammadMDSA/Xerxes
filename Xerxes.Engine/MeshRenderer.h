#pragma once
#include "GameObjectComponent.h"
#include "ResourceManager.h"
#include <string>
#include "boost/serialization/access.hpp"

class MeshRenderer : public GameObjectComponent
{
	XCOMP(MeshRenderer)
public:
	MeshRenderer();
	MeshRenderer(const MeshRenderer& other);
	~MeshRenderer() = default;

	// Inherited via GameObjectComponent
	virtual void OnRender(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* context) override;
	virtual void OnStart() override;
	virtual void OnAwake() override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnGizmo(ImGuizmo::OPERATION manipulationOperation, ImGuizmo::MODE manipulationMode) override;
	virtual void OnInspector() override;
	virtual void OnDestroy() override;

	void SetModelResourceId(GameResourceId id);

	MeshRenderer& operator=(const MeshRenderer& other);
private:
	friend class boost::serialization::access;

	std::unique_ptr<DirectX::CommonStates> states;
	XProperty(GameResourceId, meshResourceId)
	XProperty(bool, usingPrimitives)
	XProperty(GameResourceId, effectResourceId)

	void RenderPrimitive();
	void RenderModel();

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		boost::serialization::void_cast_register<MeshRenderer, GameObjectComponent>(
			static_cast<MeshRenderer*>(NULL),
			static_cast<GameObjectComponent*>(NULL)
			);
		//ar& boost::serialization::base_object<GameObjectComponent>(*this);
		ar& meshResourceId;
		ar& usingPrimitives;
		ar& effectResourceId;
	}
};

