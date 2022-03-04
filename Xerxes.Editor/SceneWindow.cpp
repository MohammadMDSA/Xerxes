#include "pch.h"
#include "SceneWindow.h"
#include "Libs/imgui/imgui.h"
#include "RootManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SceneWindow::SceneWindow(int id) :
	EditorWindow(id, "Scene"),
	cameraPitch(XM_2PI / 4.f),
	cameraYaw(0.f)
{

	backgroundAlpha = 0.f;
}

void SceneWindow::SetCamera(Camera* camera)
{
	this->camera = camera;
}

Camera* SceneWindow::GetCamera()
{
	return camera;
}

void SceneWindow::OnGUI()
{
}

int SceneWindow::GetCustomWindowFlags()
{
	return ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking;
}

void SceneWindow::Update(float deltaTime)
{
	if (!hasFocus)
		return;
	auto input = RootManager::GetInstance()->GetInputManager();

	if (input->GetRightButton() || input->GetMiddleButton())
	{
		if (input->GetMouseMode() == Mouse::MODE_ABSOLUTE)
		{
			input->SetMouseMode(Mouse::MODE_RELATIVE);
			return;
		}
	}

	// Handle panngin
	if (input->GetMiddleButton() && input->GetMouseMode() == Mouse::MODE_RELATIVE)
	{
		auto preRot = camera->GetRotation();
		auto pos = camera->GetPosition();
		auto matRot = Matrix::CreateFromQuaternion(preRot);

		auto xDelta = ROTATION_GAIN * -input->GetMouseX();
		auto yDelta = ROTATION_GAIN * input->GetMouseY();

		auto right = matRot.Right();
		right.Normalize();
		auto up = matRot.Up();
		up.Normalize();
		auto direction = right * xDelta + up * yDelta;
		direction.Normalize();
		camera->SetPositionV(direction * deltaTime * PAN_GAIN + pos);
		return;
	}

	// Handle right click self rotation and movement
	if (input->GetRightButton() && input->GetMouseMode() == Mouse::MODE_RELATIVE)
	{

		auto cameraYawDelta = ROTATION_GAIN * -input->GetMouseX();
		auto cameraPitchDelta = ROTATION_GAIN * -input->GetMouseY();

		// Handle Self rotation
		auto preRot = camera->GetRotation();
		auto rot = Matrix::CreateFromQuaternion(preRot);
		rot = DirectX::XMMatrixMultiply(rot, Matrix::CreateFromAxisAngle(rot.Right(), DirectX::XMConvertToRadians(cameraPitchDelta)));
		rot *= Matrix::CreateFromAxisAngle(Vector3::Up, DirectX::XMConvertToRadians(cameraYawDelta));
		camera->SetRotation(Quaternion::CreateFromRotationMatrix(rot), false);

		cameraYaw += cameraYawDelta;
		cameraPitch += cameraPitchDelta;

		// Handle ghost movement
		auto pos = camera->GetPosition();
		Vector3 movement;
		auto keyboard = input->GetKeyboard();
		auto kb = keyboard->GetState();
		if (kb.W)
			movement.y += 1;
		if (kb.S)
			movement.y -= 1;
		if (kb.A)
			movement.x -= 1;
		if (kb.D)
			movement.x += 1;
		if (kb.E)
			movement.z += 1;
		if (kb.Q)
			movement.z -= 1;
		auto forward = rot.Forward();
		forward.Normalize();
		auto right = rot.Right();
		right.Normalize();
		auto up = rot.Up();
		up.Normalize();
		auto direction = forward * movement.y + right * movement.x + up * movement.z;
		direction.Normalize();
		camera->SetPositionV(direction * deltaTime * MOVEMENT_GAIN + pos);
		return;
	}
	if (!input->GetMiddleButton() && !input->GetRightButton())
		input->SetMouseMode(Mouse::MODE_ABSOLUTE);
}
