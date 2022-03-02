#include "pch.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;

Transform::Transform() :
	position(Vector3::Zero),
	scale(Vector3::One),
	rotation(Quaternion::Identity),
	world(Matrix::Identity),
	rotationX(0.f),
	rotationY(0.f),
	rotationZ(0.f)
{
}

DirectX::SimpleMath::Matrix Transform::GetWorldMatrix() const
{
	return world;
}

DirectX::SimpleMath::Vector3 Transform::GetPosition()
{
	return position;
}

DirectX::SimpleMath::Vector3 Transform::GetScale()
{
	return scale;
}

float Transform::GetRotationX()
{
	return rotationX;
}

float Transform::GetRotationY()
{
	return rotationY;
}

float Transform::GetRotationZ()
{
	return rotationZ;
}

const DirectX::SimpleMath::Quaternion& Transform::GetRotation() const
{
	return rotation;
}

void Transform::SetRotation(DirectX::SimpleMath::Quaternion quat, bool updateWorld)
{
	this->rotation = quat;
	rotationX = DirectX::XMConvertToDegrees(rotation.x);
	rotationY = DirectX::XMConvertToDegrees(rotation.y);
	rotationZ = DirectX::XMConvertToDegrees(rotation.z);
	if (updateWorld)
		this->CreateWorld();
}

void Transform::SetRotationX(float x, bool updateWorld)
{
	auto rot = Matrix::CreateFromQuaternion(rotation);
	rot = DirectX::XMMatrixMultiply(rot, Matrix::CreateFromAxisAngle(rot.Right(), DirectX::XMConvertToRadians(x - rotationX)));
	rotation = Quaternion::CreateFromRotationMatrix(rot);
	rotationX = x;
	if (updateWorld)
		this->CreateWorld();
}

void Transform::SetRotationY(float y, bool updateWorld)
{
	auto rot = Matrix::CreateFromQuaternion(rotation);
	rot = DirectX::XMMatrixMultiply(rot, Matrix::CreateFromAxisAngle(rot.Up(), DirectX::XMConvertToRadians(y - rotationY)));
	rotation = Quaternion::CreateFromRotationMatrix(rot);
	rotationY = y;
	if (updateWorld)
		this->CreateWorld();
}

void Transform::SetRotationZ(float z, bool updateWorld)
{
	auto rot = Matrix::CreateFromQuaternion(rotation);
	rot = DirectX::XMMatrixMultiply(rot, Matrix::CreateFromAxisAngle(rot.Forward(), DirectX::XMConvertToRadians(z - rotationZ)));
	rotation = Quaternion::CreateFromRotationMatrix(rot);
	rotationZ = z;
	if (updateWorld)
		this->CreateWorld();
}

void Transform::SetPositionV(const Vector3& position, bool updateWorld)
{
	this->position = position;
	if (updateWorld)
		this->CreateWorld();
}

void Transform::SetPosition(float x, float y, float z, bool updateWorld)
{
	this->position = Vector3(x, y, z);
	if (updateWorld)
		this->CreateWorld();
}

void Transform::SetScaleV(Vector3 scale, bool updateWorld)
{
	this->scale = scale;
	if (updateWorld)
		this->CreateWorld();
}

void Transform::SetScale(float x, float y, float z, bool updateWorld)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	if (updateWorld)
		this->CreateWorld();
}


const DirectX::SimpleMath::Vector3& Transform::Forward() const
{
	return world.Forward();
}

const DirectX::SimpleMath::Vector3& Transform::Right() const
{
	return world.Right();
}

const DirectX::SimpleMath::Vector3& Transform::Up() const
{
	return world.Up();
}

inline void Transform::CreateWorld()
{

	Matrix rot = Matrix::CreateFromQuaternion(rotation);

	auto rest = Matrix::CreateWorld(position, rot.Forward(), rot.Up());

	world = DirectX::XMMatrixMultiply(Matrix::CreateScale(scale), rest);

}
