#include "pch.h"
#include "Camera.h"
#include "Libs/imgui/imgui.h"

using namespace DirectX::SimpleMath;

Camera::Camera() :
	nearPlane(0.001f),
	farPlane(100.f),
	fieldOfView(80),
	isPerspective(true),
	outputWidth(1500.f),
	outputHeight(900.f),
	transform(Transform()),
	orthoGraphicSize(10)
{
}

void Camera::SetOutputSize(const float& width, const float& height)
{
	outputWidth = width;
	outputHeight = height;
	CreateProjection();
}

void Camera::SetIsPerspective(bool isPerspective)
{
	this->isPerspective = isPerspective;
	CreateProjection();
}

void Camera::SetFieldOfVeiw(float fov)
{
	fieldOfView = fov;
	if (isPerspective)
		CreateProjection();
}

void Camera::SetNearPlane(float nearPlane)
{
	this->nearPlane = nearPlane;
	CreateProjection();
}

void Camera::SetFarPlane(float farPlane)
{
	this->farPlane = farPlane;
	CreateProjection();
}

const DirectX::SimpleMath::Matrix& Camera::GetProjection() const
{
	return projection;
}

const DirectX::SimpleMath::Matrix& Camera::GetView() const
{
	return view;
}

void Camera::OnGui()
{
	int currentItem = isPerspective ? 0 : 1;
	if (ImGui::Combo("View Type", &currentItem, "Perspective\0Orthographic\0\0"))
	{
		isPerspective = currentItem == 0 ? true : false;
		CreateProjection();
	}
	if (isPerspective)
	{
		if (ImGui::DragFloat("FoV", &fieldOfView, 0.1, 1.f, 179.9f, "%.2f"))
		{
			CreateProjection();
		}
	}
	else
	{
		if (ImGui::DragFloat("Ortho Size", &orthoGraphicSize, 0.1, 0.01, 100))
		{
			CreateProjection();
		}
	}
	if (ImGui::DragFloat("Near Plane", &nearPlane, 0.1, 0.01, farPlane - 0.001))
	{
		CreateProjection();
	}
	if (ImGui::DragFloat("Far Plane", &farPlane, 0.1, nearPlane + 0.001, 100000))
	{
		CreateProjection();
	}

}

void Camera::CreateProjection()
{
	if (isPerspective)
		projection = Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(fieldOfView), outputWidth / outputHeight, nearPlane, farPlane);
	else
		projection = Matrix::CreateOrthographic(outputWidth / orthoGraphicSize / ORTHO_SIZE_DIVISOR, outputHeight / orthoGraphicSize / ORTHO_SIZE_DIVISOR, nearPlane, farPlane);
}

void Camera::CreateView()
{
	auto target = transform.Forward();
	auto pos = transform.GetPosition();
	target += pos;
	view = Matrix::CreateLookAt(pos, target, transform.Up());
	//view = Matrix::CreateLookAt(pos, Vector3(0, 0, 0.001), Vector3::Up);

}

DirectX::SimpleMath::Vector2 Camera::GetOuputSize()
{
	return DirectX::SimpleMath::Vector2(outputWidth, outputWidth);
}

bool Camera::IsPerspective()
{
	return isPerspective;
}

const float& Camera::GetFieldOfView() const
{
	return fieldOfView;
}

const float& Camera::GetNearPlane() const
{
	return nearPlane;
}

const float& Camera::GetFarPlane() const
{
	return farPlane;
}

DirectX::SimpleMath::Vector3 Camera::GetPosition()
{
	return transform.GetPosition();
}

DirectX::SimpleMath::Quaternion Camera::GetRotation()
{
	return transform.GetRotation();
}

float Camera::GetRotationX()
{
	return transform.GetRotationX();
}

float Camera::GetRotationY()
{
	return transform.GetRotationY();
}

float Camera::GetRotationZ()
{
	return transform.GetRotationZ();
	CreateView();
}

const DirectX::SimpleMath::Vector3& Camera::GetUp() const
{
	return transform.Up();
}

const DirectX::SimpleMath::Vector3& Camera::GetRight() const
{
	return transform.Right();
}

const DirectX::SimpleMath::Vector3& Camera::GetForward() const
{
	return transform.Forward();
}

void Camera::SetRotation(DirectX::SimpleMath::Quaternion quat, bool updateWorld)
{
	transform.SetRotation(quat, updateWorld);
	CreateView();
}

void Camera::SetRotationX(float x, bool updateWorld)
{
	transform.SetRotationX(x, updateWorld);
	CreateView();
}

void Camera::SetRotationY(float y, bool updateWorld)
{
	transform.SetRotationY(y, updateWorld);
	CreateView();
}

void Camera::SetRotationZ(float z, bool updateWorld)
{
	transform.SetRotationZ(z, updateWorld);
	CreateView();
}

void Camera::SetPositionV(Vector3 position, bool updateWorld)
{
	transform.SetPositionV(position, updateWorld);
	CreateView();
}

void Camera::SetPosition(float x, float y, float z, bool updateWorld)
{
	transform.SetPosition(x, y, z, updateWorld);
	CreateView();
}
