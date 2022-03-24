#pragma once
#include "Libs/EnTT/entt.hpp"
#include "boost/serialization/access.hpp"
#include "boost/serialization/split_member.hpp"

class GameObject;
struct Transform
{

public:
	Transform();

	DirectX::SimpleMath::Matrix			GetWorldMatrix() const;
	DirectX::SimpleMath::Matrix			GetUnscaledWorld() const;
	DirectX::SimpleMath::Vector3		GetPosition();
	DirectX::SimpleMath::Vector3		GetScale();
	float								GetRotationX();
	float								GetRotationY();
	float								GetRotationZ();
	DirectX::SimpleMath::Vector3		GetWorldPosition();
	float								GetWorldRotationX();
	float								GetWorldRotationY();
	float								GetWorldRotationZ();
	DirectX::SimpleMath::Vector3		GetWorldScale();

	const DirectX::SimpleMath::Quaternion& GetRotation() const;
	const DirectX::SimpleMath::Quaternion& GetWorldRotation() const;

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


	const DirectX::SimpleMath::Vector3& Forward() const;
	const DirectX::SimpleMath::Vector3& Right() const;
	const DirectX::SimpleMath::Vector3& Up() const;

	void									SetParent(Transform* parent);
	Transform* GetParent();
	const std::vector<Transform*>* GetChildren() const;

	GameObject* const						GetGameObject();

private:
	friend class boost::serialization::access;
	friend class GameObject;

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
	Transform* parent;
	std::vector<Transform*>					children;

	entt::entity							gameObjectId;

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		ar& position.x;
		ar& position.y;
		ar& position.z;
		ar& scale.x;
		ar& scale.y;
		ar& scale.z;
		ar& rotation.x;
		ar& rotation.y;
		ar& rotation.z;
		ar& gameObjectId;
		ar& parent;
		ar& children.size();
		for (auto child : children)
		{
			ar& child;
		}
	}
	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		ar& position.x;
		ar& position.y;
		ar& position.z;
		ar& scale.x;
		ar& scale.y;
		ar& scale.z;
		ar& rotation.x;
		ar& rotation.y;
		ar& rotation.z;
		ar& gameObjectId;

		ar& parent;
		int childrenCount;
		ar& childrenCount;
		
		children.clear();
		for (size_t i = 0; i < childrenCount; i++)
		{
			Transform* child;
			ar& child;
			children.push_back(child);
		}

		CreateWorld();
		SetEulerAngels();
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
};

