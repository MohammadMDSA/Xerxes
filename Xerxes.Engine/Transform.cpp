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

void Transform::SetRotationX(float x, bool updateWorld)
{
	auto rot = Matrix::CreateFromQuaternion(rotation);
	rot = DirectX::XMMatrixMultiply(rot, Matrix::CreateFromAxisAngle(rot.Right(), DirectX::XMConvertToRadians(x - rotationX)));
	rotation = Quaternion::CreateFromRotationMatrix(rot);
	rotationX = x;
	if (updateWorld)
		CreateWorld();
}

void Transform::SetRotationY(float y, bool updateWorld)
{
	auto rot = Matrix::CreateFromQuaternion(rotation);
	rot = DirectX::XMMatrixMultiply(rot, Matrix::CreateFromAxisAngle(rot.Up(), DirectX::XMConvertToRadians(y - rotationY)));
	rotation = Quaternion::CreateFromRotationMatrix(rot);
	rotationY = y;
	if (updateWorld)
		CreateWorld();
}

void Transform::SetRotationZ(float z, bool updateWorld)
{
	auto rot = Matrix::CreateFromQuaternion(rotation);
	rot = DirectX::XMMatrixMultiply(rot, Matrix::CreateFromAxisAngle(rot.Forward(), DirectX::XMConvertToRadians(z - rotationZ)));
	rotation = Quaternion::CreateFromRotationMatrix(rot);
	rotationZ = z;
	if (updateWorld)
		CreateWorld();
}

void Transform::SetPositionV(Vector3 position, bool updateWorld)
{
	this->position = position;
	if (updateWorld)
		CreateWorld();
}

void Transform::SetPosition(float x, float y, float z, bool updateWorld)
{
	position.x = x;
	position.y = y;
	position.z = z;
	if (updateWorld)
		CreateWorld();
}

void Transform::SetScaleV(Vector3 scale, bool updateWorld)
{
	this->scale = scale;
	if (updateWorld)
		CreateWorld();
}

void Transform::SetScale(float x, float y, float z, bool updateWorld)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	if (updateWorld)
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
	
	Matrix rot = Matrix::CreateFromQuaternion(rotation);

	//auto rot = Matrix::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(rotation.y), DirectX::XMConvertToRadians(rotation.x), DirectX::XMConvertToRadians(rotation.z));
	
	auto rest = Matrix::CreateWorld(position, rot.Forward(), rot.Up());

	world = DirectX::XMMatrixMultiply(Matrix::CreateScale(scale), rest);
}
