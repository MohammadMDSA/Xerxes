#pragma once

#include "Transform.h"

class GameObject
{
public:
	Transform transform;

	void					OnStart(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context);
	void					OnAwake();
	void					OnUpdate();
	void					OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void					OnDestroy();

	virtual void			OnGizmo();
	void					OnInspectorBase();
protected:

	virtual void			OnInspector();

private:

	std::unique_ptr<DirectX::GeometricPrimitive>    m_shape;

};

