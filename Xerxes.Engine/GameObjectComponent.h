#pragma once

#include <memory>
#include "GameObject.h"

class GameObject;

class GameObjectComponent
{
public:
	virtual void OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, ID3D11DeviceContext1* context) = 0;
	virtual void OnStart(ID3D11Device1* device, ID3D11DeviceContext1* context) = 0;
	virtual void OnAwake() = 0;
	virtual void OnUpdate(float deltaTime) = 0;
	virtual void OnGizmo() = 0;
	virtual void OnInspector() = 0;
	virtual void OnDestroy() = 0;
protected:
	friend class GameObject;
	std::shared_ptr<GameObject> gameObject;
private:

};

