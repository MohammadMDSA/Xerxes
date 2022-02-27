#include "pch.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;

Transform::Transform() :
	position(Vector3::Zero),
	scale(Vector3::One),
	rotation(Quaternion::Identity)
{
}

DirectX::SimpleMath::Matrix Transform::GetWorldMatrix() const
{
	return world;
}

DirectX::SimpleMath::Vector3 Transform::GetRotation()
{
	return rotation;
}

DirectX::SimpleMath::Vector3 Transform::GetPosition()
{
	return position;
}

DirectX::SimpleMath::Vector3 Transform::GetScale()
{
	return scale;
}

void Transform::SetRotationV(Vector3 angles)
{
	rotation = angles;
	CreateWorld();
}

void Transform::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
	CreateWorld();
}

void Transform::SetPositionV(Vector3 position)
{
	this->position = position;
	CreateWorld();
}

void Transform::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	CreateWorld();
}

void Transform::SetScaleV(Vector3 scale)
{
	this->scale = scale;
	CreateWorld();
}

void Transform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	CreateWorld();
}


inline DirectX::SimpleMath::Vector3 Transform::Forward()
{
	return world.Forward();
}

inline DirectX::SimpleMath::Vector3 Transform::Right()
{
	return world.Right();
}

inline DirectX::SimpleMath::Vector3 Transform::Up()
{
	return world.Up();
}

void Transform::CreateWorld()
{
	auto ss = Matrix::CreateScale(scale);
	auto rot = Matrix::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(rotation.y), DirectX::XMConvertToRadians(rotation.x), DirectX::XMConvertToRadians(rotation.z));
	auto rest = Matrix::CreateWorld(position, rot.Forward(), rot.Up());
	world = DirectX::XMMatrixMultiply(ss, rest);
}
