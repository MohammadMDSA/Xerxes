
//
// Game.cpp
//

#include "pch.h"
#include "Editor.h"
#include <stdexcept>

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/imgui_impl_win32.h"
#include "Libs/imgui/imgui_impl_dx11.h"
#include "Libs/imgui/ImGuizmo.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Scene.h"
#include "Libs/ImGuiFileDialog/ImGuiFileDialog.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Editor::Editor() noexcept :
	m_window(nullptr),
	m_outputWidth(1500),
	m_outputHeight(900),
	m_imguiActive(false),
	showDemo(true),
	rootManager(nullptr),
	sceneWidth(1),
	sceneHeight(1)
{
	m_deviceResources = std::make_unique<DX::DeviceResources>();
	m_deviceResources->RegisterDeviceNotify(this);
	windowResource = new EditorWindowGraphicResource();
}

Editor::~Editor()
{
	rootManager->Destroy();
	delete sceneWindow;
	delete inspectorWindow;
}

// Initialize the Direct3D resources required to run.
void Editor::Initialize(HWND window, int width, int height)
{
	m_window = window;
	m_outputWidth = std::max(width, 1);
	m_outputHeight = std::max(height, 1);

	m_deviceResources->SetWindow(window, m_outputWidth, m_outputHeight);


	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

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

	rootManager->GetInputManager()->GetMouse()->SetWindow(window);

	rootManager->GetCameraManager()->CraeteCamera();
	auto camera = rootManager->GetCameraManager()->GetActiveCamera();
	camera->SetPosition(2, 2, 2);
	sceneWindow->SetCamera(rootManager->GetCameraManager()->GetActiveCamera());

	GetDefaultSize(sceneWidth, sceneHeight);

}

// Executes the basic game loop.
void Editor::Tick()
{
	m_timer.Tick([&]()
		{
			Update(m_timer);
		});

	Render();
}

// Updates the world.
void Editor::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	rootManager->Update(elapsedTime);
	sceneWindow->Update(elapsedTime);
	inspectorWindow->Update(elapsedTime);

	// Handling layout
	int newSceneWidth = sceneWindow->GetWidth();
	int newSceneHeight = sceneWindow->GetHeight();
	if (newSceneWidth != sceneWidth || newSceneHeight != sceneHeight)
	{
		sceneHeight = newSceneHeight;
		sceneWidth = newSceneWidth;
		RootManager::GetInstance()->GetCameraManager()->SetOutputSize(sceneWidth, sceneHeight);
		windowResource->Initialize(RootManager::GetInstance()->GetResourceManager()->GetDevice(), sceneWidth, sceneHeight);
	}

	elapsedTime;
}

// Draws the scene.
void Editor::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}



	auto context = m_deviceResources->GetD3DDeviceContext();
	auto depthStencil = m_deviceResources->GetDepthStencilView();
	windowResource->ClearRenderTarget(context, depthStencil, 0.392156899f, 0.584313750f, 0.929411829f, 1);
	windowResource->SetRenderTarget(context, depthStencil, m_deviceResources->GetRenderTargetView());


	m_deviceResources->PIXBeginEvent(L"Render");
	auto camera = sceneWindow->GetCamera();

	auto view = camera->GetView();
	auto proj = camera->GetProjection();
	auto sceneManager = rootManager->GetSceneManager();

	sceneManager->OnRender(view, proj);
	sceneWindow->OnRender(view, proj);

	m_deviceResources->PIXEndEvent();



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
	ImGui::Image((void*)windowResource->GetShaderResourceView(), ImVec2(sceneWidth, sceneHeight));
	ImGui::SetCursorPos(curPos);

	// Drawing scene gizmo
	auto min = ImGui::GetWindowContentRegionMin();
	ImGuizmo::SetRect(sceneWindow->GetPosX() + min.x, sceneWindow->GetPosY() + min.y, sceneWidth, sceneHeight);
	sceneManager->OnGizmo(sceneWindow->GetManipulationOperation(), sceneWindow->GetManipulationMode());


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
	Clear();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Editor::Clear()
{
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();

	// Clear the views.
	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	// Set the viewport.
	float width = sceneWindow->GetWidth(), height = sceneHeight;
	//if (width > 0 && height > 0)
	{
		CD3D11_VIEWPORT viewport(0.f, 0.f, static_cast<float>(width), static_cast<float>(height));
		context->RSSetViewports(1, &viewport);
	}
	/*else
	{
		CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
		context->RSSetViewports(1, &viewport);
	}*/
}

// Message handlers
void Editor::OnActivated()
{
	// TODO: Game is becoming active window.
}

void Editor::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void Editor::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
}

void Editor::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize).
}

void Editor::OnWindowMoved()
{
	auto r = m_deviceResources->GetOutputSize();
	m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Editor::OnWindowSizeChanged(int width, int height)
{
	if (!m_deviceResources->WindowSizeChanged(width, height))
		return;
	m_outputWidth = std::max(width, 1);
	m_outputHeight = std::max(height, 1);

	CreateWindowSizeDependentResources();
}

// Properties
void Editor::GetDefaultSize(int& width, int& height) const noexcept
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 1500;
	height = 900;
}

void Editor::OnDeviceLost()
{
	// Cleaning up imgui
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	m_imguiActive = false;

}

void Editor::OnDeviceRestored()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

void Editor::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();

	device;
}

void Editor::CreateWindowSizeDependentResources()
{
	InitializeImgui();
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto device = m_deviceResources->GetD3DDevice();
	RootManager::GetInstance()->GetResourceManager()->SetDevice(device);
	RootManager::GetInstance()->GetResourceManager()->SetDeviceContext(context);

	windowResource->Initialize(device, sceneWidth, sceneHeight);
}

void Editor::InitializeImgui()
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

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_window);
	ImGui_ImplDX11_Init(m_deviceResources->GetD3DDevice(), m_deviceResources->GetD3DDeviceContext());

	clear_color[0] = 0.45f;

	clear_color[1] = 0.55f;

	clear_color[2] = 0.60f;
	m_imguiActive = true;
}

void Editor::AppBarMenus()
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
				auto currentScene = rootManager->GetSceneManager()->GetCurrentScene();
				auto obj = new GameObject(currentScene);
				obj->OnStart();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	AddItem();
}
void Editor::AddItem()
{
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

			int modelId = rootManager->GetResourceManager()->CreateModel(std::wstring(filePathName.begin(), filePathName.end()));
		}

		ImGuiFileDialog::Instance()->Close();
	}
}

void Editor::MakeDockSpace()
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
