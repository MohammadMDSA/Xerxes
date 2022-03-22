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
	manipulationOperation(ImGuizmo::OPERATION::TRANSLATE),
	manipulationMode(ImGuizmo::MODE::LOCAL)
{
	backgroundAlpha = 1.f;
	auto resourceManager = RootManager::GetInstance()->GetResourceManager();
	auto device = resourceManager->GetDevice();

	states = std::make_unique<DirectX::CommonStates>(device);
	effectId = resourceManager->ResourceGroup<EffectResource>::GetByName("Position Color Effect")->GetId();
	SetDimansion(800, 600);
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

	ImGui::SetNextWindowBgAlpha(0.7);
	ImGui::BeginChild("Tools", ImVec2(0, 40), true, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
	
	// Creating table layout
	ImGui::BeginTable("tab", 2, ImGuiTableFlags_SizingFixedSame | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_BordersInnerV);

	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);

	ImGui::RadioButton("World", (int*)&manipulationMode, ImGuizmo::MODE::WORLD);
	ImGui::SameLine();
	ImGui::RadioButton("Local", (int*)&manipulationMode, ImGuizmo::MODE::LOCAL);
	
	ImGui::TableSetColumnIndex(1);
	ImGui::RadioButton("Position", (int*)&manipulationOperation, ImGuizmo::OPERATION::TRANSLATE);
	ImGui::SameLine();
	ImGui::RadioButton("Rotation", (int*)&manipulationOperation, ImGuizmo::OPERATION::ROTATE);
	ImGui::SameLine();
	ImGui::RadioButton("Scale", (int*)&manipulationOperation, ImGuizmo::OPERATION::SCALE);
	
	ImGui::EndTable();
	ImGui::EndChild();
}

int SceneWindow::GetCustomWindowFlags()
{
	return 0; /*ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove;*/

}

ImGuizmo::OPERATION SceneWindow::GetManipulationOperation()
{
	return manipulationOperation;
}

ImGuizmo::MODE SceneWindow::GetManipulationMode()
{
	return manipulationMode;
}

void SceneWindow::Update(float deltaTime)
{
	if (!isHovered && !moveingCamera)
		return;
	auto input = RootManager::GetInstance()->GetInputManager();
	auto kb = input->GetKeyboard()->GetState();

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
	if (input->GetMiddleButton())
	{
		moveingCamera = true;
		auto preRot = camera->GetRotation();
		auto pos = camera->GetPosition();
		auto matRot = Matrix::CreateFromQuaternion(preRot);

		auto xDelta = ROTATION_GAIN * -input->GetMouseDeltaX();
		auto yDelta = ROTATION_GAIN * input->GetMouseDeltaY();

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
	if (input->GetRightButton())
	{
		moveingCamera = true;
		auto cameraYawDelta = ROTATION_GAIN * -input->GetMouseDeltaX();
		auto cameraPitchDelta = ROTATION_GAIN * -input->GetMouseDeltaY();

		// Handle Self rotation
		auto preRot = camera->GetRotation();
		auto rot = Matrix::CreateFromQuaternion(preRot);
		rot = DirectX::XMMatrixMultiply(rot, Matrix::CreateFromAxisAngle(rot.Right(), DirectX::XMConvertToRadians(cameraPitchDelta)));
		rot *= Matrix::CreateFromAxisAngle(Vector3::Up, DirectX::XMConvertToRadians(cameraYawDelta));
		camera->SetRotation(Quaternion::CreateFromRotationMatrix(rot));

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
}

void SceneWindow::OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{

	auto resourceManager = RootManager::GetInstance()->GetResourceManager();
	auto context = resourceManager->GetDeviceContext();
	auto batch = resourceManager->GetDefaultBatch();
	auto effect = resourceManager->ResourceGroup<EffectResource>::GetById(effectId)->GetResource();

	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->RSSetState(states->CullNone());

	effect->SetView(view);
	effect->SetProjection(proj);
	effect->SetWorld(DirectX::SimpleMath::Matrix::Identity);
	effect->Apply(context);

	context->IASetInputLayout(resourceManager->GetVertexPositionColorInputLayout());

	batch->Begin();

	Vector3 xaxis(1.f, 0.f, 0.f);
	Vector3 yaxis(0.f, 0.f, 1.f);
	Vector3 origin = Vector3::Zero;

	const int count = 100;
	DirectX::XMVECTORF32 color;

	for (int i = (-count); i <= count; ++i)
	{
		color = (i == 0 ? Colors::White : Colors::Gray);
		auto scale = xaxis * i;
		VertexPositionColor v1(scale - (count * yaxis), color);
		VertexPositionColor v2(scale + (count * yaxis), color);
		batch->DrawLine(v1, v2);
	}

	for (int i = -count; i <= count; i++)
	{
		color = (i == 0 ? Colors::White : Colors::Gray);
		auto scale = yaxis * i;
		VertexPositionColor v1(scale - (count * xaxis), color);
		VertexPositionColor v2(scale + (count * xaxis), color);
		batch->DrawLine(v1, v2);
	}

	batch->End();
}
