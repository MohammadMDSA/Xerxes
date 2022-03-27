#pragma once

#include <memory>
#include <string>

#include "XPreprocessors.h"
#include "IInspectorDrawer.h"

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/ImGuizmo.h"
#include "Libs/EnTT/entt.hpp"

#include "boost/serialization/base_object.hpp"
#include "boost/serialization/access.hpp"

class GameObject;

class GameObjectComponent : public IInspectorDrawer
{
public:
	GameObjectComponent(std::string name);
	GameObjectComponent(const GameObjectComponent& other);

	virtual void OnRender(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* context) = 0;
	virtual void OnStart() = 0;
	virtual void OnAwake() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnGizmo(ImGuizmo::OPERATION manipulationOperation, ImGuizmo::MODE manipulationMode) = 0;
	virtual void OnInspector() = 0;
	virtual void OnDestroy() = 0;
	inline std::string GetName() { return name; };

protected:
	friend class boost::serialization::access;
	friend class GameObject;

	GameObject*			gameObject = nullptr;
	std::string			name;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& gameObject;
		ar& name;
	}
};

