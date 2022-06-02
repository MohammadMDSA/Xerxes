#pragma once
#include "EditorWindow.h"
#include "Camera.h"
#include "GameResource.h"
#include "Types.h"

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/ImGuizmo.h"

using namespace Xerxes::Engine;

namespace Xerxes
{
	namespace Editor
	{
		namespace Panels
		{
			class SceneWindow : public EditorWindow
			{
			public:
				SceneWindow(int id);

				void					SetCamera(Camera* camera);
				Camera* GetCamera();

				virtual void			OnGUI() override;
				virtual void			Update(float deltaTime) override;
				void					OnRender(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

				ImGuizmo::OPERATION		GetManipulationOperation();
				ImGuizmo::MODE			GetManipulationMode();

				inline const float& GetCameraDistance() const { return cameraDistance; }

			protected:
				// Inherited via EditorWindow
				virtual int				GetCustomWindowFlags() override;

			private:

				Camera* camera;
				const float				ROTATION_GAIN = 1.f;
				const float				MOVEMENT_GAIN = 5.f;
				const float				ROTATION_AROUND_FORWARD = 2.f;
				const float				ROTATION_AROUND_ROTATION_GAIN = 0.1f;
				const float				PAN_GAIN = 6.f;

				bool					moveingCamera;
				GameResourceId			effectId;
				std::unique_ptr<DirectX::CommonStates> states;

				ImGuizmo::OPERATION		manipulationOperation;
				ImGuizmo::MODE			manipulationMode;

				float					cameraDistance;
			};

		}
	}
}