#pragma once

#include <memory>
#include <string>

#include "GameObject.h"

class GameObject;

class GameObjectComponent : public IInspectorDrawer
{
public:
	virtual void OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, ID3D11DeviceContext* context) = 0;
	virtual void OnStart() = 0;
	virtual void OnAwake() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnGizmo() = 0;
	virtual void OnInspector() = 0;
	virtual void OnDestroy() = 0;
	virtual std::string GetName() = 0;

protected:
	friend class GameObject;
	GameObject*			gameObject;

};

