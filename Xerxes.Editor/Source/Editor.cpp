//
// Game.cpp
//

#include "pch.h"
#include "Editor.h"
#include <stdexcept>

#include "Libs/imgui/imgui.h"
#include "Libs/imgui/imgui_impl_win32.h"
#include "Libs/imgui/imgui_impl_dx11.h"
#include "RootManager.h"
#include "Camera.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Editor::Editor() noexcept :
	m_window(nullptr),
	m_outputWidth(1500),
	m_outputHeight(900),
	m_featureLevel(D3D_FEATURE_LEVEL_9_1),
	m_imguiActive(false),
	showDemo(true)
{
}

Editor::~Editor()
{
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

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:

	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);

	this->sceneWindow = new SceneWindow(1);
	this->inspectorWindow = new InspectorWindow(2);
	this->go = new GameObject();
	RootManager* p = RootManager::GetInstance();

	p->GetInputManager()->GetMouse()->SetWindow(window);

	p->GetCameraManager()->CraeteCamera();
	p->GetCameraManager()->GetActiveCamera()->SetPosition(2, 2, 2);
	sceneWindow->SetCamera(p->GetCameraManager()->GetActiveCamera());

	GetDefaultSize(sceneWidth, sceneHeight);

	CreateDevice();

	CreateResources();
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

	m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(sceneWindow->GetWidth()) / float(sceneWindow->GetHeight()), 0.1f, 10.f);

	sceneWindow->Update(elapsedTime);
	inspectorWindow->Update(elapsedTime);
	// TODO: Add your game logic here.
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

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemo)
		ImGui::ShowDemoWindow(&showDemo);

		//// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		//{
		//	static float f = 0.0f;
		//	static int counter = 0;

		//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		//	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		//	ImGui::Checkbox("Another Window", &show_another_window);

		//	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//	float pos[3] = { position.x, position.y, position.z };
		//	ImGui::DragFloat3("Position", pos, 0.01f);
		//	position = (XMFLOAT3)pos;
		//	ImGui::ColorEdit3("clear color", clear_color); // Edit 3 floats representing a color

		//	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//		counter++;
		//	ImGui::SameLine();
		//	ImGui::Text("counter = %d", counter);

		//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//	ImGui::End();
		//}

		//// 3. Show another simple window.
		//if (show_another_window)
		//{
		//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		//	ImGui::Text("Hello from another window!");
		//	if (ImGui::Button("Close Me"))
		//		show_another_window = false;
		//	ImGui::End();
		//}
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
	sceneWindow->BeginWindow();
	sceneWindow->EndWindow();
	inspectorWindow->SetPosition(m_outputWidth - inspectorWindow->GetWidth(), 0.f);
	inspectorWindow->SetDimansion(inspectorWindow->GetWidth(), m_outputHeight);
	inspectorWindow->BeginWindow();
	go->OnInspectorBase();
	inspectorWindow->EndWindow();


	ImGui::Begin("Camera Inspector", &showCameraInspector);
	auto camera = sceneWindow->GetCamera();
	camera->OnGui();
	ImGui::End();

	// 
	// Rendering
	ImGui::Render();

	Clear();


	go->OnRender(camera->GetView(), camera->GetProjection(), m_d3dContext.Get());
	//go->OnRender(camera->GetView(), Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		//float(m_outputWidth) / float(m_outputHeight), 0.1f, 10.f));
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	// TODO: Add your rendering code here.

	Present();
}

// Helper method to clear the back buffers.
void Editor::Clear()
{
	// Clear the views.
	m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
	m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	// Set the viewport.
	float width = sceneWindow->GetWidth(), height = sceneWindow->GetHeight();
	if (width > 0 && height > 0)
	{
		CD3D11_VIEWPORT viewport(sceneWindow->GetPosX(), sceneWindow->GetPosY(), static_cast<float>(width), static_cast<float>(height));
		m_d3dContext->RSSetViewports(1, &viewport);
	}
	else
	{
		CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
		m_d3dContext->RSSetViewports(1, &viewport);
	}
}

// Presents the back buffer contents to the screen.
void Editor::Present()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = m_swapChain->Present(1, 0);

	// If the device was reset we must completely reinitialize the renderer.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		OnDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);
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

void Editor::OnWindowSizeChanged(int width, int height)
{
	m_outputWidth = std::max(width, 1);
	m_outputHeight = std::max(height, 1);

	CreateResources();
}

// Properties
void Editor::GetDefaultSize(int& width, int& height) const noexcept
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 1500;
	height = 900;
}

// These are the resources that depend on the device.
void Editor::CreateDevice()
{
	UINT creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	static const D3D_FEATURE_LEVEL featureLevels[] =
	{
		// TODO: Modify for supported Direct3D feature levels
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	// Create the DX11 API device object, and get a corresponding context.
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	DX::ThrowIfFailed(D3D11CreateDevice(
		nullptr,                            // specify nullptr to use the default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		static_cast<UINT>(std::size(featureLevels)),
		D3D11_SDK_VERSION,
		device.ReleaseAndGetAddressOf(),    // returns the Direct3D device created
		&m_featureLevel,                    // returns feature level of device created
		context.ReleaseAndGetAddressOf()    // returns the device immediate context
	));

#ifndef NDEBUG
	ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(device.As(&d3dDebug)))
	{
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
		{
#ifdef _DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// TODO: Add more message IDs here as needed.
			};
			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = static_cast<UINT>(std::size(hide));
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

	DX::ThrowIfFailed(device.As(&m_d3dDevice));
	DX::ThrowIfFailed(context.As(&m_d3dContext));

}

// Allocate all memory resources that change on a window SizeChanged event.
void Editor::CreateResources()
{
	// Clear the previous window size specific context.
	m_d3dContext->OMSetRenderTargets(0, nullptr, nullptr);
	m_renderTargetView.Reset();
	m_depthStencilView.Reset();
	m_d3dContext->Flush();

	const UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
	const UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
	const DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	const DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	constexpr UINT backBufferCount = 2;

	// If the swap chain already exists, resize it, otherwise create one.
	if (m_swapChain)
	{
		HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			OnDeviceLost();

			// Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
			// and correctly set up the new device.
			return;
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
	}
	else
	{
		// First, retrieve the underlying DXGI Device from the D3D Device.
		ComPtr<IDXGIDevice1> dxgiDevice;
		DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

		// Identify the physical adapter (GPU or card) this device is running on.
		ComPtr<IDXGIAdapter> dxgiAdapter;
		DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

		// And obtain the factory object that created it.
		ComPtr<IDXGIFactory2> dxgiFactory;
		DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

		// Create a descriptor for the swap chain.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = backBufferWidth;
		swapChainDesc.Height = backBufferHeight;
		swapChainDesc.Format = backBufferFormat;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = backBufferCount;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
		fsSwapChainDesc.Windowed = TRUE;

		// Create a SwapChain from a Win32 window.
		DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
			m_d3dDevice.Get(),
			m_window,
			&swapChainDesc,
			&fsSwapChainDesc,
			nullptr,
			m_swapChain.ReleaseAndGetAddressOf()
		));

		// This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
		DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
	}

	// Obtain the backbuffer for this window which will be the final 3D rendertarget.
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	// Create a view interface on the rendertarget to use on bind.
	DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

	// Allocate a 2-D surface as the depth/stencil buffer and
	// create a DepthStencil view on this surface to use on bind.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

	ComPtr<ID3D11Texture2D> depthStencil;
	DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

	PostResourceCreation();
}

void Editor::PostResourceCreation()
{
	InitializeImgui();
	go->OnStart(m_d3dDevice.Get(), m_d3dContext.Get());
}

void Editor::OnDeviceLost()
{
	// Cleaning up imgui
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	m_imguiActive = false;
	

	m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain.Reset();
	m_d3dContext.Reset();
	m_d3dDevice.Reset();

	CreateDevice();

	CreateResources();
}

void Editor::InitializeImgui()
{
	if (m_imguiActive)
		return;
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_window);
	ImGui_ImplDX11_Init(m_d3dDevice.Get(), m_d3dContext.Get());

	clear_color[0] = 0.45f;

	clear_color[1] = 0.55f;

	clear_color[2] = 0.60f;
	m_imguiActive = true;
}
