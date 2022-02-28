#pragma once

struct Transform
{

public:
	Transform();

	//Delegate<float, float>				OnWorldCreated;

	DirectX::SimpleMath::Matrix			GetWorldMatrix() const;
	DirectX::SimpleMath::Vector3		GetPosition();
	DirectX::SimpleMath::Vector3		GetScale();
	float								GetRotationX();
	float								GetRotationY();
	float								GetRotationZ();

	void								SetRotationX(float x, bool updateWorld = true);
	void								SetRotationY(float y, bool updateWorld = true);
	void								SetRotationZ(float z, bool updateWorld = true);
	void								SetPositionV(const DirectX::SimpleMath::Vector3& position, bool updateWorld = true);
	void								SetPosition(float x, float y, float z, bool updateWorld = true);
	void								SetScaleV(DirectX::SimpleMath::Vector3 scale, bool updateWorld = true);
	void								SetScale(float x, float y, float z, bool updateWorld = true);


	DirectX::SimpleMath::Vector3		Forward();
	DirectX::SimpleMath::Vector3		Right();
	DirectX::SimpleMath::Vector3		Up();


private:
	inline void							CreateWorld();

	DirectX::SimpleMath::Vector3		position;
	DirectX::SimpleMath::Vector3		scale;
	DirectX::SimpleMath::Quaternion		rotation;
	float								rotationX;
	float								rotationY;
	float								rotationZ;

	DirectX::SimpleMath::Matrix			world;
};

