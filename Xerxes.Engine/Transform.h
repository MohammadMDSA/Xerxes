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
	const DirectX::SimpleMath::Quaternion&	GetRotation() const;

	void								SetRotation(DirectX::SimpleMath::Quaternion quat, bool updateWorld = true);
	void								SetRotationX(float x, bool updateWorld = true);
	void								SetRotationY(float y, bool updateWorld = true);
	void								SetRotationZ(float z, bool updateWorld = true);
	void								SetWorld(DirectX::SimpleMath::Matrix world);
	void								SetPositionV(const DirectX::SimpleMath::Vector3& position, bool updateWorld = true);
	void								SetPosition(float x, float y, float z, bool updateWorld = true);
	void								SetScaleV(DirectX::SimpleMath::Vector3 scale, bool updateWorld = true);
	void								SetScale(float x, float y, float z, bool updateWorld = true);


	const DirectX::SimpleMath::Vector3&		Forward() const;
	const DirectX::SimpleMath::Vector3&		Right() const;
	const DirectX::SimpleMath::Vector3&		Up() const;

private:
	inline void								CreateWorld();

	DirectX::SimpleMath::Vector3			position;
	DirectX::SimpleMath::Vector3			scale;
	DirectX::SimpleMath::Quaternion			rotation;
	float									rotationX;
	float									rotationY;
	float									rotationZ;

	DirectX::SimpleMath::Matrix				world;
};

