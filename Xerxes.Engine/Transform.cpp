#include "pch.h"
#include "Transform.h"

using namespace DirectX::SimpleMath;

Transform::Transform(GameObject* obj) :
	position(Vector3::Zero),
	scale(Vector3::One),
	rotation(Quaternion::Identity),
	world(Matrix::Identity),
	rotationX(0.f),
	rotationY(0.f),
	rotationZ(0.f),
	parent(nullptr),
	gameObject(obj)
{
}

DirectX::SimpleMath::Matrix Transform::GetWorldMatrix() const
{
	if (parent)
		return world * parent->GetWorldMatrix();
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

DirectX::SimpleMath::Vector3 Transform::GetWorldPosition()
{
	if (!parent)
		return position;
	return parent->GetWorldPosition() + position;
}

float Transform::GetWorldRotationX()
{
	if (!parent)
		return rotationX;
	auto nWorld = GetWorldMatrix();
	return DirectX::XMConvertToDegrees(atan2f(nWorld._23, nWorld._33)) + rotationX;
}

float Transform::GetWorldRotationY()
{
	if (!parent)
		return rotationY;
	auto nWorld = GetWorldMatrix();
	return DirectX::XMConvertToDegrees(atan2f(-world._13, sqrtf(world._23 * world._23 + world._33 * world._33))) + rotationY;
}

float Transform::GetWorldRotationZ()
{
	if (!parent)
		return rotationZ;
	auto nWorld = GetWorldMatrix();
	return DirectX::XMConvertToDegrees(atan2f(world._12, world._11)) + rotationZ;
}

const DirectX::SimpleMath::Quaternion& Transform::GetRotation() const
{
	return rotation;
}

const DirectX::SimpleMath::Quaternion& Transform::GetWorldRotation() const
{
	if (!parent)
		return rotation;
	return rotation * parent->rotation;
}

void Transform::SetRotation(DirectX::SimpleMath::Quaternion quat)
{
	this->rotation = quat;
	this->CreateWorld();
	SetEulerAngels();
}

void Transform::SetWorldRotation(DirectX::SimpleMath::Quaternion quat)
{
	if (!parent)
		SetRotation(quat);
	auto parRot = parent->GetRotation();
	parRot.Inverse(parRot);
	quat *= parRot;
	rotation = quat;
	CreateWorld();
	SetEulerAngels();
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

void Transform::SetWorld(DirectX::SimpleMath::Matrix world)
{
	if (!parent)
	{
		this->world = world;
	}
	else {
		auto pworld = parent->GetWorldMatrix();
		this->world = world * pworld.Invert();
	}
	this->world.Decompose(scale, rotation, position);
	SetEulerAngels();
}

void Transform::SetPositionV(const Vector3& position, bool updateWorld)
{
	this->position = position;
	if (updateWorld)
		this->CreateWorld();
}

void Transform::SetWorldPositionV(const DirectX::SimpleMath::Vector3& position, bool updateWorld)
{
	if (!parent)
		SetPositionV(position, updateWorld);
	SetPositionV(position - parent->GetPosition(), updateWorld);
}

void Transform::SetPosition(float x, float y, float z, bool updateWorld)
{
	this->position = Vector3(x, y, z);
	if (updateWorld)
		this->CreateWorld();
}

void Transform::SetWorldPosition(float x, float y, float z, bool updateWorld)
{
	if (!parent)
		SetPosition(x, y, z, updateWorld);
	auto parentP = parent->GetPosition();
	SetPosition(x - parentP.x, y - parentP.y, z - parentP.z, updateWorld);
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

void Transform::SetParent(Transform* parent)
{
	auto wrd = GetWorldMatrix();
	if (this->parent && parent != this->parent)
	{
		this->parent->children.erase(std::remove(this->parent->children.begin(), this->parent->children.end(), this));
	}
	this->parent = parent;
	SetWorld(wrd);
	if (!parent)
		return;
	if (std::find(parent->children.begin(), parent->children.end(), this) != parent->children.end())
		return;
	parent->children.push_back(this);
}

Transform* Transform::GetParent()
{
	return this->parent;
}

const std::vector<Transform*>* Transform::GetChildren() const
{
	return &children;
}

inline void Transform::CreateWorld()
{

	Matrix rot = Matrix::CreateFromQuaternion(rotation);

	auto rest = Matrix::CreateWorld(position, rot.Forward(), rot.Up());

	world = DirectX::XMMatrixMultiply(Matrix::CreateScale(scale), rest);

}

void Transform::DecomposeParent(DirectX::SimpleMath::Vector3& scale, DirectX::SimpleMath::Quaternion& rotation, DirectX::SimpleMath::Vector3& translation)
{
	if (!parent)
	{
		scale = Vector3::One;
		rotation = Quaternion::Identity;
		translation = Vector3::Zero;
	}
	parent->GetWorldMatrix().Decompose(scale, rotation, translation);
}

void Transform::SetEulerAngels()
{
	rotationX = DirectX::XMConvertToDegrees(atan2f(world._23, world._33));
	rotationY = DirectX::XMConvertToDegrees(atan2f(-world._13, sqrtf(world._23 * world._23 + world._33 * world._33)));
	rotationZ = DirectX::XMConvertToDegrees(atan2f(world._12, world._11));
}

GameObject* const Transform::GetGameObject()
{
	return this->gameObject;
}
