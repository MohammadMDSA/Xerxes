#pragma once
#include "Delegate.h"

struct Transform
{

public:
	Transform();
	
	DirectX::SimpleMath::Matrix			GetWorldMatrix() const;
	DirectX::SimpleMath::Vector3		GetPosition();
	DirectX::SimpleMath::Vector3		GetScale();
	float								GetRotationX();
	float								GetRotationY();
	float								GetRotationZ();

	void								SetRotationX(float x, bool updateWorld = true);
	void								SetRotationY(float y, bool updateWorld = true);
	void								SetRotationZ(float z, bool updateWorld = true);
	void								SetPositionV(DirectX::SimpleMath::Vector3 position, bool updateWorld = true);
	void								SetPosition(float x, float y, float z, bool updateWorld = true);
	void								SetScaleV(DirectX::SimpleMath::Vector3 scale, bool updateWorld = true);
	void								SetScale(float x, float y, float z, bool updateWorld = true);
		
	void								CreateWorld();

	inline DirectX::SimpleMath::Vector3 Forward();
	inline DirectX::SimpleMath::Vector3 Right();
	inline DirectX::SimpleMath::Vector3 Up();

	//Delegate<void(void)>				OnWorldCreated;


private:

	DirectX::SimpleMath::Vector3		position;
	DirectX::SimpleMath::Vector3		scale;
	DirectX::SimpleMath::Quaternion		rotation;
	float								rotationX;
	float								rotationY;
	float								rotationZ;

	DirectX::SimpleMath::Matrix			world;
};

