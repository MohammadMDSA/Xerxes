#include "pch.h"
#include "SceneWindow.h"
#include "Libs/imgui/imgui.h"
#include "RootManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SceneWindow::SceneWindow(int id) :
	EditorWindow(id, "Scene"),
	moveingCamera(false),
	camera(nullptr),
	effectId(-1)
{
	backgroundAlpha = 0.f;
	auto resourceManager = RootManager::GetInstance()->GetResourceManager();
	auto device = resourceManager->GetDevice();

	states = std::make_unique<DirectX::CommonStates>(device);
	auto effect = new BasicEffect(device);
	effect->SetVertexColorEnabled(true);
	effectId = resourceManager->CreateEffect(effect);
	auto eff = resourceManager->ResourceGroup<EffectResource>::GetById(effectId);
	CreateInputLayoutFromEffect<VertexPositionColor>(device, eff->GetResource(), &resourceManager->vertexPositionInputLayout);
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
	return ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;
	return 0;
}

void SceneWindow::Update(float deltaTime)
{
	if (!isHovered && !moveingCamera)
		return;
	auto input = RootManager::GetInstance()->GetInputManager();
	auto kb = input->GetKeyboard()->GetState();

	if (input->GetRightButton() || input->GetMiddleButton()/* || (input->GetLeftButton() && kb.LeftAlt)*/)
	{
		if (input->GetMouseMode() == Mouse::MODE_ABSOLUTE)
		{
			input->SetMouseMode(Mouse::MODE_RELATIVE);
			return;
		}
	}

	/*if (input->GetLeftButton() && kb.LeftAlt && input->GetMouseMode() == Mouse::MODE_RELATIVE)
	{
		moveingCamera = true;

		auto cameraYawDelta = ROTATION_AROUND_ROTATION_GAIN * -input->GetMouseX();
		auto cameraPitchDelta = ROTATION_AROUND_ROTATION_GAIN * -input->GetMouseY();

		auto preRot = camera->GetRotation();
		auto rot = Matrix::CreateFromQuaternion(preRot);
		rot = DirectX::XMMatrixMultiply(rot, Matrix::CreateFromAxisAngle(rot.Right(), DirectX::XMConvertToRadians(cameraPitchDelta)));
		rot *= Matrix::CreateFromAxisAngle(Vector3::Up, DirectX::XMConvertToRadians(cameraYawDelta));

		auto pos = camera->GetPosition();
		auto world = camera->GetWorld();

		auto forward = camera->GetForward();
		auto lookAtDir = Vector3(forward.x * ROTATION_AROUND_FORWARD, forward.y * ROTATION_AROUND_FORWARD, forward.z * ROTATION_AROUND_FORWARD);
		auto lookAt = lookAtDir + pos;

		world *= Matrix::CreateTranslation(-lookAt);
		world *= rot;
		world *= Matrix::CreateTranslation(lookAt);
		camera->SetWorld(world);

		return;
	}*/

	// Handle panngin
	if (input->GetMiddleButton() && input->GetMouseMode() == Mouse::MODE_RELATIVE)
	{
		moveingCamera = true;
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
		moveingCamera = true;
		auto cameraYawDelta = ROTATION_GAIN * -input->GetMouseX();
		auto cameraPitchDelta = ROTATION_GAIN * -input->GetMouseY();

		// Handle Self rotation
		auto preRot = camera->GetRotation();
		auto rot = Matrix::CreateFromQuaternion(preRot);
		rot = DirectX::XMMatrixMultiply(rot, Matrix::CreateFromAxisAngle(rot.Right(), DirectX::XMConvertToRadians(cameraPitchDelta)));
		rot *= Matrix::CreateFromAxisAngle(Vector3::Up, DirectX::XMConvertToRadians(cameraYawDelta));
		camera->SetRotation(Quaternion::CreateFromRotationMatrix(rot), false);

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
	moveingCamera = false;
	input->SetMouseMode(Mouse::MODE_ABSOLUTE);
}

void SceneWindow::OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{

	auto resourceManager = RootManager::GetInstance()->GetResourceManager();
	auto context = resourceManager->GetDeviceContext();
	auto batch = resourceManager->GetDefaultBatch();
	auto effect = resourceManager->ResourceGroup<EffectResource>::GetById(effectId)->GetResource();

	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthNone(), 0);
	context->RSSetState(states->CullNone());

	effect->SetView(view);
	effect->SetProjection(proj);
	effect->SetWorld(DirectX::SimpleMath::Matrix::Identity);
	effect->Apply(context);

	context->IASetInputLayout(resourceManager->vertexPositionInputLayout.Get());

	batch->Begin();

	Vector3 xaxis(2.f, 0.f, 0.f);
	Vector3 yaxis(0.f, 0.f, 2.f);
	Vector3 origin = Vector3::Zero;

	constexpr size_t divisions = 20;

	for (size_t i = 0; i <= divisions; ++i)
	{
		float fPercent = float(i) / float(divisions);
		fPercent = (fPercent * 2.0f) - 1.0f;

		Vector3 scale = xaxis * fPercent + origin;

		VertexPositionColor v1(scale - yaxis, Colors::White);
		VertexPositionColor v2(scale + yaxis, Colors::White);
		batch->DrawLine(v1, v2);
	}

	for (size_t i = 0; i <= divisions; i++)
	{
		float fPercent = float(i) / float(divisions);
		fPercent = (fPercent * 2.0f) - 1.0f;

		Vector3 scale = yaxis * fPercent + origin;

		VertexPositionColor v1(scale - xaxis, Colors::White);
		VertexPositionColor v2(scale + xaxis, Colors::White);
		batch->DrawLine(v1, v2);
	}

	batch->End();
}
