
//
// Game.cpp
//

#include "pch.h"
#include "Editor.h"
#include <stdexcept>
#include <fstream>

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/imgui_impl_win32.h"
#include "Libs/imgui/imgui_impl_dx11.h"
#include "Libs/imgui/ImGuizmo.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "Libs/ImGuiFileDialog/ImGuiFileDialog.h"
#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace DX;
using namespace Xerxes;
using namespace Xerxes::Engine::Graphics::Device;
using namespace Xerxes::Editor::Panels;

using Microsoft::WRL::ComPtr;

Xerxes::Editor::Editor::Editor() noexcept :
	m_window(nullptr),
	m_outputWidth(1500),
	m_outputHeight(900),
	m_imguiActive(false),
	showDemo(false),
	rootManager(nullptr)
{
}

Xerxes::Editor::Editor::~Editor()
{
	rootManager->Destroy();
	delete sceneWindow;
	delete inspectorWindow;
}

// Initialize the Direct3D resources required to run.
void Xerxes::Editor::Editor::Initialize(HWND window, int width, int height)
{
	m_window = window;
	m_outputWidth = std::max(width, 1);
	m_outputHeight = std::max(height, 1);

	auto renderer = XRenderer();

	renderer->RegisterDeviceNotify(this);
	renderer->SetWindow(window, width, height);
	renderer->CreateResource();

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:

	////////////////////////// REMOVE below


	this->sceneWindow = new SceneWindow(1);
	this->inspectorWindow = new InspectorWindow(2);
	this->hierarchyWindow = new HierarchyWindow(3);
	this->resourceWindow = new ResourceWindow(4);
	this->sceneWindow->SetPosition(0.f, 0.f);

	this->rootManager = RootManager::GetInstance();

	/////////////////////////// Remove above

	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);

	XResourceM()->LoadAllSubdirectoriesResources("assets");
	XResourceM()->InitializeResources();

	rootManager->GetInputManager()->GetMouse()->SetWindow(window);

	rootManager->GetCameraManager()->CraeteCamera();
	auto camera = rootManager->GetCameraManager()->GetActiveCamera();
	camera->SetPosition(2, 2, 2);
	sceneWindow->SetCamera(rootManager->GetCameraManager()->GetActiveCamera());

}

// Executes the basic game loop.
void Xerxes::Editor::Editor::Tick()
{
	m_timer.Tick([&]()
		{
			Update(m_timer);
		});

	Render();
}

// Updates the world.
void Xerxes::Editor::Editor::Update(StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	rootManager->Update(elapsedTime);
	sceneWindow->Update(elapsedTime);
	inspectorWindow->Update(elapsedTime);
	XSceneM()->Update(elapsedTime);

	// Handling layout
	int newSceneWidth = xmax((int)sceneWindow->GetWidth(), 1);
	int newSceneHeight = xmax((int)sceneWindow->GetHeight(), 1);
	int sceneWidth, sceneHeight;

	auto renderer = XRenderer();
	renderer->GetRenderDimansion(sceneWidth, sceneHeight);
	if (newSceneWidth != sceneWidth || newSceneHeight != sceneHeight)
	{
		sceneHeight = newSceneHeight;
		sceneWidth = newSceneWidth;
		RootManager::GetInstance()->GetCameraManager()->SetOutputSize(sceneWidth, sceneHeight);
		renderer->SetRenderDimansion(sceneWidth, sceneHeight);
	}
	elapsedTime;
}

// Draws the scene.
void Xerxes::Editor::Editor::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}
	
	auto camera = XCameraM()->GetActiveCamera();
	auto view = camera->GetView();
	auto proj = camera->GetProjection();

	auto renderer = XRenderer();
	renderer->Render();

	sceneWindow->OnRender(view, proj);
	
	RenderGUI();
	
	renderer->Clear();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	renderer->Present();
}

void Xerxes::Editor::Editor::RenderGUI()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	MakeDockSpace();
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemo)
		ImGui::ShowDemoWindow(&showDemo);
	auto selectionManager = rootManager->GetSelectionManager();
	auto selected = selectionManager->GetSelectedInspectorDrawer();

	sceneWindow->BeginWindow();

	// Drawing scene texture
	auto curPos = ImGui::GetCursorPos();

	int sceneWidth, sceneHeight;
	XRenderer()->GetRenderDimansion(sceneWidth, sceneHeight);
	ImGui::Image((void*)XRenderer()->GetViewTexture(), ImVec2(sceneWidth, sceneHeight));
	ImGui::SetCursorPos(curPos);

	// Drawing scene gizmo
	auto min = ImGui::GetWindowContentRegionMin();
	ImGuizmo::SetRect(sceneWindow->GetPosX() + min.x, sceneWindow->GetPosY() + min.y, sceneWidth, sceneHeight);
	XSceneM()->OnGizmo(sceneWindow->GetManipulationOperation(), sceneWindow->GetManipulationMode());


	sceneWindow->EndWindow();

	// Drawing Inspector
	inspectorWindow->BeginWindow();
	if (selected)
		selected->OnInspector();
	inspectorWindow->EndWindow();

	// Drawing Hierarchy
	hierarchyWindow->BeginWindow();
	hierarchyWindow->EndWindow();

	// Drawing Resource
	resourceWindow->BeginWindow();
	resourceWindow->EndWindow();

	AppBarMenus();


	ImGui::Begin("Camera Inspector", &showCameraInspector);

	auto camera = XCameraM()->GetActiveCamera();
	camera->OnGui();

	ImGui::End();

	// 
	// Rendering
	ImGui::Render();



	/*if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}*/
}

// Message handlers
void Xerxes::Editor::Editor::OnActivated()
{
	// TODO: Game is becoming active window.
}

void Xerxes::Editor::Editor::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void Xerxes::Editor::Editor::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
}

void Xerxes::Editor::Editor::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize).
}

void Xerxes::Editor::Editor::OnWindowMoved()
{
}

void Xerxes::Editor::Editor::OnWindowSizeChanged(int width, int height)
{
	if (!XRenderer()->SetOutputSize(width, height))
		return;
	m_outputWidth = std::max(width, 1);
	m_outputHeight = std::max(height, 1);

	CreateWindowSizeDependentResources();
}

// Properties
void Xerxes::Editor::Editor::GetDefaultSize(int& width, int& height) const noexcept
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 1500;
	height = 900;
}

void Xerxes::Editor::Editor::OnDeviceLost()
{
	// Cleaning up imgui
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	m_imguiActive = false;

}

void Xerxes::Editor::Editor::OnDeviceRestored(ID3D11DeviceContext* context, ID3D11Device* device)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

void Xerxes::Editor::Editor::CreateDeviceDependentResources()
{
}

void Xerxes::Editor::Editor::CreateWindowSizeDependentResources()
{
	InitializeImgui();
}

void Xerxes::Editor::Editor::InitializeImgui()
{
	if (m_imguiActive)
		return;
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.f);
	ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 8.f);
	ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 6.f);
	ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 3.f);

	io.ConfigWindowsMoveFromTitleBarOnly = true;

	auto renderer = XRenderer();
	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_window);
	ImGui_ImplDX11_Init(renderer->GetDevice(), renderer->GetDeviceContext());

	clear_color[0] = 0.45f;

	clear_color[1] = 0.55f;

	clear_color[2] = 0.60f;
	m_imguiActive = true;
}

void Xerxes::Editor::Editor::AppBarMenus()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "CTRL+N"))
			{
				ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".sdkmesh", ".");
			}
			if (ImGui::MenuItem("New GameObject"))
			{
				auto obj = GameObject::Create();
				XSelectionM()->SetSelectedGameObject(obj);
				obj->OnStart();
			}
			if (ImGui::MenuItem("Save"))
			{
				std::ofstream ofs("filename");
				auto scene = rootManager->GetSceneManager()->GetCurrentScene();
				{
					boost::archive::text_oarchive oa(ofs);
					oa << scene;
				}
			}
			if (ImGui::MenuItem("Load"))
			{
				std::ifstream ifs("filename");
				Scene* scene;
				{
					boost::archive::text_iarchive ia(ifs);
					ia >> scene;
					if (scene)
					{
						rootManager->GetSceneManager()->SetCurrentScene(scene);
					}
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	AddItem();
}
void Xerxes::Editor::Editor::AddItem()
{
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

			rootManager->GetResourceManager()->CreateModel(filePathName);
		}

		ImGuiFileDialog::Instance()->Close();
	}
}

void Xerxes::Editor::Editor::MakeDockSpace()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


	ImGui::Begin("DockSpace Demo", nullptr, window_flags);
	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	ImGui::End();
}
