#pragma once

#include <memory>
#include <string>

#include "XPreprocessors.h"
#include "IInspectorDrawer.h"
#include "TypeSerialization.h"
#include "XReference.h"
#include "XPreprocessors.h"

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/ImGuizmo.h"
#include "Libs/EnTT/entt.hpp"

#include "boost/serialization/base_object.hpp"
#include "boost/serialization/access.hpp"
#include <boost/pfr.hpp>
#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <rttr/registration.h>
#include <rttr/type.h>
#include <rttr/registration_friend.h>
#include <rttr/rttr_enable.h>


using namespace rttr;
using namespace Xerxes::Engine::Core::Reference;

class GameObject;

class GameObjectComponent : public XReference, public IInspectorDrawer
{
	XClass(GameObjectComponent, XReference)

public:
	GameObjectComponent();
	GameObjectComponent(const GameObjectComponent& other);

	virtual void OnRender(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj, ID3D11DeviceContext* context) = 0;
	virtual void OnStart() = 0;
	virtual void OnAwake() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnGizmo(ImGuizmo::OPERATION manipulationOperation, ImGuizmo::MODE manipulationMode) = 0;
	virtual void OnInspector() = 0;
	virtual void OnDestroy() = 0;
	inline virtual std::string GetName() = 0;

protected:
	friend class boost::serialization::access;
	friend class GameObject;

	GameObject*			gameObject = nullptr;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& gameObject;
	}
private:
	RTTR_REGISTRATION_FRIEND
};

