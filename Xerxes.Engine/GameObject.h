#pragma once

#include "Transform.h"
#include <memory>

class GameObject
{
public:
	Transform transform;

	void					OnStart(ID3D11Device1* device, ID3D11DeviceContext1* context);
	void					OnAwake();
	void					OnUpdate();
	void					OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, ID3D11DeviceContext1* context);
	void					OnDestroy();

	virtual void			OnGizmo();
	void					OnInspectorBase();
protected:

	virtual void			OnInspector();

private:

	std::unique_ptr<DirectX::GeometricPrimitive>    m_shape;


	std::unique_ptr<DirectX::CommonStates> m_states;
	std::unique_ptr<DirectX::IEffectFactory> m_fxFactory;
	std::unique_ptr<DirectX::GeometricPrimitive> m_model;
};

