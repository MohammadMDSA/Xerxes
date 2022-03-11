#pragma once

class GameObject;
struct Transform
{

public:
	Transform(GameObject*);

	//Delegate<float, float>				OnWorldCreated;

	DirectX::SimpleMath::Matrix			GetWorldMatrix() const;
	DirectX::SimpleMath::Vector3		GetPosition();
	DirectX::SimpleMath::Vector3		GetScale();
	float								GetRotationX();
	float								GetRotationY();
	float								GetRotationZ();
	DirectX::SimpleMath::Vector3		GetWorldPosition();
	float								GetWorldRotationX();
	float								GetWorldRotationY();
	float								GetWorldRotationZ();

	const DirectX::SimpleMath::Quaternion&	GetRotation() const;
	const DirectX::SimpleMath::Quaternion&	GetWorldRotation() const;

	void								SetRotation(DirectX::SimpleMath::Quaternion quat);
	void								SetWorldRotation(DirectX::SimpleMath::Quaternion quat);
	void								SetRotationX(float x, bool updateWorld = true);
	void								SetRotationY(float y, bool updateWorld = true);
	void								SetRotationZ(float z, bool updateWorld = true);
	void								SetWorld(DirectX::SimpleMath::Matrix world);
	void								SetPositionV(const DirectX::SimpleMath::Vector3& position, bool updateWorld = true);
	void								SetWorldPositionV(const DirectX::SimpleMath::Vector3& position, bool updateWorld = true);
	void								SetPosition(float x, float y, float z, bool updateWorld = true);
	void								SetWorldPosition(float x, float y, float z, bool updateWorld = true);
	void								SetScaleV(DirectX::SimpleMath::Vector3 scale, bool updateWorld = true);
	void								SetScale(float x, float y, float z, bool updateWorld = true);


	const DirectX::SimpleMath::Vector3&		Forward() const;
	const DirectX::SimpleMath::Vector3&		Right() const;
	const DirectX::SimpleMath::Vector3&		Up() const;

	void									SetParent(Transform* parent);
	Transform*								GetParent();
	const std::vector<Transform*>*			GetChildren() const;

	GameObject* const						GetGameObject();

private:
	inline void								CreateWorld();
	void									DecomposeParent(DirectX::SimpleMath::Vector3& scale, DirectX::SimpleMath::Quaternion& rotation, DirectX::SimpleMath::Vector3& translation);

	void									SetEulerAngels();

	DirectX::SimpleMath::Vector3			position;
	DirectX::SimpleMath::Vector3			scale;
	DirectX::SimpleMath::Quaternion			rotation;
	float									rotationX;
	float									rotationY;
	float									rotationZ;

	DirectX::SimpleMath::Matrix				world;
	Transform*								parent;
	std::vector<Transform*>					children;

	GameObject*								gameObject;
};

