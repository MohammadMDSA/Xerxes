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
#include <shobjidl.h> 
#include "MeshRenderer.h"
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
	rootManager(nullptr)
{
	m_deviceResources = std::make_unique<DX::DeviceResources>();
	m_deviceResources->RegisterDeviceNotify(this);
}

Editor::~Editor()
{
	rootManager->Destroy();
	delete sceneWindow;
	delete inspectorWindow;
	delete go;
}

// Initialize the Direct3D resources required to run.
void Editor::Initialize(HWND window, int width, int height)
{
	m_window = window;
	m_outputWidth = std::max(width, 1);
	m_outputHeight = std::max(height, 1);

	m_deviceResources->SetWindow(window, m_outputWidth, m_outputHeight);

	////////////////////////// REMOVE below


	this->sceneWindow = new SceneWindow(1);
	this->inspectorWindow = new InspectorWindow(2);
	this->hierarchyWindow = new HierarchyWindow(3);
	this->resourceWindow = new ResourceWindow(4);
	this->sceneWindow->SetDimansion(100.f, 100.f);
	this->sceneWindow->SetPosition(0.f, 0.f);
	this->go = new GameObject();
	go->transform.SetPosition(0, 0, 2);
	this->go1 = new GameObject();
	go1->transform.SetPosition(0, 0, -2);
	this->go2 = new GameObject();
	go2->transform.SetPosition(2, 0, 0);
	this->go3 = new GameObject();
	go3->transform.SetPosition(-2, 0, 0);
	this->go4 = new GameObject();
	go4->transform.SetPosition(0, 2, 0);
	this->go5 = new GameObject();
	go5->transform.SetPosition(0, -2, 0);
	this->rootManager = RootManager::GetInstance();

	/////////////////////////// Remove above


	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:

	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);

	rootManager->GetInputManager()->GetMouse()->SetWindow(window);

	rootManager->GetCameraManager()->CraeteCamera();
	rootManager->GetCameraManager()->GetActiveCamera();
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
	int newSceneWidth = m_outputWidth - inspectorWindow->GetWidth();
	int newSceneHeight = m_outputHeight;
	if (newSceneWidth != sceneWidth || newSceneHeight != sceneHeight)
	{
		sceneHeight = newSceneHeight;
		sceneWidth = newSceneWidth;
		RootManager::GetInstance()->GetCameraManager()->SetOutputSize(sceneWidth, sceneHeight);
	}
	sceneWindow->SetPosition(0, 0);
	sceneWindow->SetDimansion(sceneWidth, sceneHeight);

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


	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemo)
		ImGui::ShowDemoWindow(&showDemo);
	auto sceneManager = rootManager->GetSceneManager();

	sceneWindow->BeginWindow();
	ImGuizmo::SetRect(0, 0, sceneWidth, sceneHeight);
	sceneManager->OnGizmo();

	sceneWindow->EndWindow();
	inspectorWindow->SetPosition(m_outputWidth - inspectorWindow->GetWidth(), 0.f);
	inspectorWindow->SetDimansion(inspectorWindow->GetWidth(), m_outputHeight);
	inspectorWindow->BeginWindow();
	sceneManager->OnInspector();
	inspectorWindow->EndWindow();
	hierarchyWindow->BeginWindow();
	hierarchyWindow->EndWindow();
	resourceWindow->BeginWindow();
	resourceWindow->EndWindow();

	AppBarMenus();


	ImGui::Begin("Camera Inspector", &showCameraInspector);
	auto camera = sceneWindow->GetCamera();
	camera->OnGui();

	ImGui::End();

	// 
	// Rendering
	ImGui::Render();

	Clear();

	m_deviceResources->PIXBeginEvent(L"Render");
	auto context = m_deviceResources->GetD3DDeviceContext();

	auto view = camera->GetView();
	auto proj = camera->GetProjection();

	sceneManager->OnRender(view, proj);

	go->OnRender(view, proj, context);
	go1->OnRender(view, proj, context);
	go2->OnRender(view, proj, context);
	go3->OnRender(view, proj, context);
	go4->OnRender(view, proj, context);
	go5->OnRender(view, proj, context);


	m_deviceResources->PIXEndEvent();

	/*if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}*/
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
	float width = sceneWindow->GetWidth(), height = sceneWindow->GetHeight();
	if (width > 0 && height > 0)
	{
		CD3D11_VIEWPORT viewport(sceneWindow->GetPosX(), sceneWindow->GetPosY(), static_cast<float>(width), static_cast<float>(height));
		context->RSSetViewports(1, &viewport);
	}
	else
	{
		CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
		context->RSSetViewports(1, &viewport);
	}
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

	go->OnStart();
	go1->OnStart();
	go2->OnStart();
	go3->OnStart();
	go4->OnStart();
	go5->OnStart();
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
				AddItem();
			}
			if (ImGui::MenuItem("New GameObject"))
			{
				auto obj = new GameObject();
				obj->OnStart();
				rootManager->GetSceneManager()->AddGameObject(obj);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Editor::AddItem()
{
	IFileOpenDialog* pFileOpen;

	// Create the FileOpenDialog object.
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
		IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

	if (SUCCEEDED(hr))
	{
		// Show the Open dialog box.
		hr = pFileOpen->Show(NULL);

		// Get the file name from the dialog box.
		if (SUCCEEDED(hr))
		{
			IShellItem* pItem;
			hr = pFileOpen->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				PWSTR pszFilePath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

				// Display the file name to the user.
				if (SUCCEEDED(hr))
				{
					auto goo = new GameObject();
					int modelId = rootManager->GetResourceManager()->CreateModel(pszFilePath);
					auto mesh = new MeshRenderer();
					goo->AddComponent(mesh);
					goo->OnStart();
					rootManager->GetSceneManager()->AddGameObject(goo);
				}
				pItem->Release();
			}
		}
		pFileOpen->Release();
	}
	CoUninitialize();
}


//void Editor::AddItem()
//{
//	ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".sdkmesh", ".");
//
//	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
//	{
//		if (ImGuiFileDialog::Instance()->IsOk())
//		{
//			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
//
//			auto goo = new GameObject();
//			int modelId = rootManager->GetResourceManager()->CreateModel(std::wstring(filePathName.begin(), filePathName.end()));
//			auto mesh = new MeshRenderer();
//			goo->AddComponent(mesh);
//			goo->OnStart();
//			rootManager->GetSceneManager()->AddGameObject(goo);
//		}
//
//		ImGuiFileDialog::Instance()->Close();
//	}
//}