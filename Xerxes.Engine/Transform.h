#pragma once

struct Transform
{

public:
	Transform();
	
	DirectX::SimpleMath::Matrix			GetWorldMatrix() const;
	DirectX::SimpleMath::Vector3		GetRotation();
	DirectX::SimpleMath::Vector3		GetPosition();
	DirectX::SimpleMath::Vector3		GetScale();

	void								SetRotationV(DirectX::SimpleMath::Vector3 angles);
	void								SetRotation(float x, float y, float z);
	void								SetPositionV(DirectX::SimpleMath::Vector3 position);
	void								SetPosition(float x, float y, float z);
	void								SetScaleV(DirectX::SimpleMath::Vector3 scale);
	void								SetScale(float x, float y, float z);
		

	inline DirectX::SimpleMath::Vector3 Forward();
	inline DirectX::SimpleMath::Vector3 Right();
	inline DirectX::SimpleMath::Vector3 Up();


private:
	void								CreateWorld();

	DirectX::SimpleMath::Vector3		position;
	DirectX::SimpleMath::Vector3		scale;
	DirectX::SimpleMath::Vector3		rotation;

	DirectX::SimpleMath::Matrix			world;
};

