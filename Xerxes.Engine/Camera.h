#pragma once
#include "Transform.h"

class Camera
{
public:
	void							SetOutputSize(const float& width, const float& height);
	void							SetIsPerspective(bool isPerspective);
	void							SetFieldOfVeiw(float fov);
	void							SetNearPlane(float near);
	void							SetFarPlane(float far);

	DirectX::SimpleMath::Vector2	GetOuputSize();
	bool							IsPerspective();
	const float&					GetFieldOfView() const;
	const float&					GetNearPlane() const;
	const float&					GetFarPlane() const;


	DirectX::SimpleMath::Vector3	GetPosition();
	DirectX::SimpleMath::Quaternion	GetRotation();
	float							GetRotationX();
	float							GetRotationY();
	float							GetRotationZ();
	DirectX::SimpleMath::Matrix		GetWorld();

	const DirectX::SimpleMath::Vector3& GetUp() const;
	const DirectX::SimpleMath::Vector3& GetRight() const;
	const DirectX::SimpleMath::Vector3& GetForward() const;
	
	void							SetRotation(DirectX::SimpleMath::Quaternion quat, bool updateWorld = true);
	void							SetRotationX(float x, bool updateWorld = true);
	void							SetRotationY(float y, bool updateWorld = true);
	void							SetRotationZ(float z, bool updateWorld = true);
	void							SetPositionV(DirectX::SimpleMath::Vector3 position, bool updateWorld = true);
	void							SetPosition(float x, float y, float z, bool updateWorld = true);
	void							SetWorld(DirectX::SimpleMath::Matrix world);

	const DirectX::SimpleMath::Matrix& GetProjection() const;
	const DirectX::SimpleMath::Matrix& GetView() const;

	void							OnGui();

	void							CreateProjection();
	void							CreateView();

private:
	friend class CameraManager;
	Transform						transform;

	Camera();

	DirectX::SimpleMath::Matrix		projection;
	DirectX::SimpleMath::Matrix		view;

	bool							isPerspective;

	float							outputWidth;
	float							outputHeight;
	float							fieldOfView;
	float							orthoGraphicSize;
	const float						ORTHO_SIZE_DIVISOR = 100;

	float							nearPlane;
	float							farPlane;
};

