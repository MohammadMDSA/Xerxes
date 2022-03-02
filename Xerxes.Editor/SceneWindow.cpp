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
	if (input->GetRightButton())
	{
		/*if (input->GetMouseMode() == Mouse::MODE_ABSOLUTE)
		{
			input->SetMouseMode(Mouse::MODE_RELATIVE);
			return;
		}*/

		auto a = input->GetMouseDeltaX();
		cameraYaw += ROTATION_GAIN * input->GetMouseDeltaX();
		cameraPitch += ROTATION_GAIN * input->GetMouseDeltaY();


		/*Vector3 lookAt = Vector3(
			0.f,
			cosf(cameraPitch),
			-cosf(cameraYaw) * sin(cameraPitch)
		);*/
		Vector3 lookAt = Vector3(
			cosf(cameraPitch),
			-cosf(cameraYaw) * sinf(cameraPitch),
			sinf(cameraYaw) * sinf(cameraPitch)
		);
		camera->SetRotation(Quaternion::CreateFromRotationMatrix(XMMatrixLookAtRH(lookAt, g_XMZero, Vector3::Up)));
	}
	else
	{
		input->SetMouseMode(Mouse::MODE_ABSOLUTE);
	}
}
