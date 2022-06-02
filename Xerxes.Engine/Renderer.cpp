#include "pch.h"
#include "Renderer.h"
#include "XPreprocessors.h"
#include "RootManager.h"

class RootManager;

using namespace DirectX;
using namespace DX;
using namespace Xerxes::Engine::Graphics::Device;
using namespace Xerxes::Engine::SubsystemManagers;

using Microsoft::WRL::ComPtr;


// Controls all the DirectX device resources.
class Xerxes::Engine::SubsystemManagers::Renderer::DeviceResources
{
public:
	static constexpr unsigned int c_FlipPresent = 0x1;
	static constexpr unsigned int c_AllowTearing = 0x2;
	static constexpr unsigned int c_EnableHDR = 0x4;

	DeviceResources(DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM,
		DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT,
		UINT backBufferCount = 2,
		D3D_FEATURE_LEVEL minFeatureLevel = D3D_FEATURE_LEVEL_11_1,
		unsigned int flags = c_FlipPresent) noexcept;
	~DeviceResources() = default;

	DeviceResources(DeviceResources&&) = default;
	DeviceResources& operator= (DeviceResources&&) = default;

	DeviceResources(DeviceResources const&) = delete;
	DeviceResources& operator= (DeviceResources const&) = delete;

	void CreateDeviceResources();
	void CreateWindowSizeDependentResources();
	void SetWindow(HWND window, int width, int height) noexcept;
	bool WindowSizeChanged(int width, int height);
	void HandleDeviceLost();
	void RegisterDeviceNotify(IDeviceNotify* deviceNotify) noexcept { m_deviceNotify = deviceNotify; }
	void Present();

	// Device Accessors.
	RECT GetOutputSize() const noexcept { return m_outputSize; }

	// Direct3D Accessors.
	auto                    GetD3DDevice() const noexcept { return m_d3dDevice.Get(); }
	auto                    GetD3DDeviceContext() const noexcept { return m_d3dContext.Get(); }
	auto                    GetSwapChain() const noexcept { return m_swapChain.Get(); }
	auto                    GetDXGIFactory() const noexcept { return m_dxgiFactory.Get(); }
	HWND                    GetWindow() const noexcept { return m_window; }
	D3D_FEATURE_LEVEL       GetDeviceFeatureLevel() const noexcept { return m_d3dFeatureLevel; }
	ID3D11Texture2D* GetRenderTarget() const noexcept { return m_renderTarget.Get(); }
	ID3D11Texture2D* GetDepthStencil() const noexcept { return m_depthStencil.Get(); }
	ID3D11RenderTargetView* GetRenderTargetView() const noexcept { return m_d3dRenderTargetView.Get(); }
	ID3D11DepthStencilView* GetDepthStencilView() const noexcept { return m_d3dDepthStencilView.Get(); }
	DXGI_FORMAT             GetBackBufferFormat() const noexcept { return m_backBufferFormat; }
	DXGI_FORMAT             GetDepthBufferFormat() const noexcept { return m_depthBufferFormat; }
	D3D11_VIEWPORT          GetScreenViewport() const noexcept { return m_screenViewport; }
	UINT                    GetBackBufferCount() const noexcept { return m_backBufferCount; }
	DXGI_COLOR_SPACE_TYPE   GetColorSpace() const noexcept { return m_colorSpace; }
	unsigned int            GetDeviceOptions() const noexcept { return m_options; }

	// Performance events
	void PIXBeginEvent(_In_z_ const wchar_t* name)
	{
		m_d3dAnnotation->BeginEvent(name);
	}

	void PIXEndEvent()
	{
		m_d3dAnnotation->EndEvent();
	}

	void PIXSetMarker(_In_z_ const wchar_t* name)
	{
		m_d3dAnnotation->SetMarker(name);
	}

private:
	void CreateFactory();
	void GetHardwareAdapter(IDXGIAdapter1** ppAdapter);
	void UpdateColorSpace();

	// Direct3D objects.
	Microsoft::WRL::ComPtr<IDXGIFactory2>               m_dxgiFactory;
	Microsoft::WRL::ComPtr<ID3D11Device1>               m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1>        m_d3dContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain1>             m_swapChain;
	Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation>   m_d3dAnnotation;

	// Direct3D rendering objects. Required for 3D.
	Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_renderTarget;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_depthStencil;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_d3dRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_d3dDepthStencilView;
	D3D11_VIEWPORT                                  m_screenViewport;

	// Direct3D properties.
	DXGI_FORMAT                                     m_backBufferFormat;
	DXGI_FORMAT                                     m_depthBufferFormat;
	UINT                                            m_backBufferCount;
	D3D_FEATURE_LEVEL                               m_d3dMinFeatureLevel;

	// Cached device properties.
	HWND                                            m_window;
	D3D_FEATURE_LEVEL                               m_d3dFeatureLevel;
	RECT                                            m_outputSize;

	// HDR Support
	DXGI_COLOR_SPACE_TYPE                           m_colorSpace;

	// DeviceResources options (see flags above)
	unsigned int                                    m_options;

	// The IDeviceNotify can be held directly as it owns the DeviceResources.
	IDeviceNotify* m_deviceNotify;
};


#ifdef __clang__
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

#pragma warning(disable : 4061)

namespace
{
#if defined(_DEBUG)
	// Check for SDK Layer support.
	inline bool SdkLayersAvailable() noexcept
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
			nullptr,
			D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
			nullptr,                    // Any feature level will do.
			0,
			D3D11_SDK_VERSION,
			nullptr,                    // No need to keep the D3D device reference.
			nullptr,                    // No need to know the feature level.
			nullptr                     // No need to keep the D3D device context reference.
		);

		return SUCCEEDED(hr);
	}
#endif

	inline DXGI_FORMAT NoSRGB(DXGI_FORMAT fmt) noexcept
	{
		switch (fmt)
		{
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:   return DXGI_FORMAT_R8G8B8A8_UNORM;
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:   return DXGI_FORMAT_B8G8R8A8_UNORM;
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:   return DXGI_FORMAT_B8G8R8X8_UNORM;
		default:                                return fmt;
		}
	}
}

// Constructor for DeviceResources.
Xerxes::Engine::SubsystemManagers::Renderer::DeviceResources::DeviceResources(
	DXGI_FORMAT backBufferFormat,
	DXGI_FORMAT depthBufferFormat,
	UINT backBufferCount,
	D3D_FEATURE_LEVEL minFeatureLevel,
	unsigned int flags) noexcept :
	m_screenViewport{},
	m_backBufferFormat(backBufferFormat),
	m_depthBufferFormat(depthBufferFormat),
	m_backBufferCount(backBufferCount),
	m_d3dMinFeatureLevel(minFeatureLevel),
	m_window(nullptr),
	m_d3dFeatureLevel(D3D_FEATURE_LEVEL_12_2),
	m_outputSize{ 0, 0, 1, 1 },
	m_colorSpace(DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709),
	m_options(flags | c_FlipPresent),
	m_deviceNotify(nullptr)
{
}

// Configures the Direct3D device, and stores handles to it and the device context.
void Xerxes::Engine::SubsystemManagers::Renderer::DeviceResources::CreateDeviceResources()
{
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	if (SdkLayersAvailable())
	{
		// If the project is in a debug build, enable debugging via SDK Layers with this flag.
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
	else
	{
		OutputDebugStringA("WARNING: Direct3D Debug Device is not available\n");
	}
#endif

	CreateFactory();

	// Determines whether tearing support is available for fullscreen borderless windows.
	if (m_options & c_AllowTearing)
	{
		BOOL allowTearing = FALSE;

		ComPtr<IDXGIFactory5> factory5;
		HRESULT hr = m_dxgiFactory.As(&factory5);
		if (SUCCEEDED(hr))
		{
			hr = factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
		}

		if (FAILED(hr) || !allowTearing)
		{
			m_options &= ~c_AllowTearing;
#ifdef _DEBUG
			OutputDebugStringA("WARNING: Variable refresh rate displays not supported");
#endif
		}
	}

	// Disable HDR if we are on an OS that can't support FLIP swap effects
	if (m_options & c_EnableHDR)
	{
		ComPtr<IDXGIFactory5> factory5;
		if (FAILED(m_dxgiFactory.As(&factory5)))
		{
			m_options &= ~c_EnableHDR;
#ifdef _DEBUG
			OutputDebugStringA("WARNING: HDR swap chains not supported");
#endif
		}
	}

	// Disable FLIP if not on a supporting OS
	if (m_options & c_FlipPresent)
	{
		ComPtr<IDXGIFactory4> factory4;
		if (FAILED(m_dxgiFactory.As(&factory4)))
		{
			m_options &= ~c_FlipPresent;
#ifdef _DEBUG
			OutputDebugStringA("INFO: Flip swap effects not supported");
#endif
		}
	}

	// Determine DirectX hardware feature levels this app will support.
	static const D3D_FEATURE_LEVEL s_featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	UINT featLevelCount = 0;
	for (; featLevelCount < static_cast<UINT>(std::size(s_featureLevels)); ++featLevelCount)
	{
		if (s_featureLevels[featLevelCount] < m_d3dMinFeatureLevel)
			break;
	}

	if (!featLevelCount)
	{
		throw std::out_of_range("minFeatureLevel too high");
	}

	ComPtr<IDXGIAdapter1> adapter;
	GetHardwareAdapter(adapter.GetAddressOf());

	// Create the Direct3D 11 API device object and a corresponding context.
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	HRESULT hr = E_FAIL;
	if (adapter)
	{
		hr = D3D11CreateDevice(
			adapter.Get(),
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			creationFlags,
			s_featureLevels,
			featLevelCount,
			D3D11_SDK_VERSION,
			device.GetAddressOf(),  // Returns the Direct3D device created.
			&m_d3dFeatureLevel,     // Returns feature level of device created.
			context.GetAddressOf()  // Returns the device immediate context.
		);
	}
#if defined(NDEBUG)
	else
	{
		throw std::runtime_error("No Direct3D hardware device found");
	}
#else
	if (FAILED(hr))
	{
		// If the initialization fails, fall back to the WARP device.
		// For more information on WARP, see:
		// http://go.microsoft.com/fwlink/?LinkId=286690
		hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
			nullptr,
			creationFlags,
			s_featureLevels,
			featLevelCount,
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			&m_d3dFeatureLevel,
			context.GetAddressOf()
		);

		if (SUCCEEDED(hr))
		{
			OutputDebugStringA("Direct3D Adapter - WARP\n");
		}
	}
#endif

	ThrowIfFailed(hr);

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
			};
			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = static_cast<UINT>(std::size(hide));
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

	ThrowIfFailed(device.As(&m_d3dDevice));
	ThrowIfFailed(context.As(&m_d3dContext));
	ThrowIfFailed(context.As(&m_d3dAnnotation));
}

// These resources need to be recreated every time the window size is changed.
void Xerxes::Engine::SubsystemManagers::Renderer::DeviceResources::CreateWindowSizeDependentResources()
{
	if (!m_window)
	{
		throw std::logic_error("Call SetWindow with a valid Win32 window handle");
	}

	// Clear the previous window size specific context.
	m_d3dContext->OMSetRenderTargets(0, nullptr, nullptr);
	m_d3dRenderTargetView.Reset();
	m_d3dDepthStencilView.Reset();
	m_renderTarget.Reset();
	m_depthStencil.Reset();
	m_d3dContext->Flush();

	// Determine the render target size in pixels.
	const UINT backBufferWidth = std::max<UINT>(static_cast<UINT>(m_outputSize.right - m_outputSize.left), 1u);
	const UINT backBufferHeight = std::max<UINT>(static_cast<UINT>(m_outputSize.bottom - m_outputSize.top), 1u);
	const DXGI_FORMAT backBufferFormat = (m_options & (c_FlipPresent | c_AllowTearing | c_EnableHDR)) ? NoSRGB(m_backBufferFormat) : m_backBufferFormat;

	if (m_swapChain)
	{
		// If the swap chain already exists, resize it.
		HRESULT hr = m_swapChain->ResizeBuffers(
			m_backBufferCount,
			backBufferWidth,
			backBufferHeight,
			backBufferFormat,
			(m_options & c_AllowTearing) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u
		);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
#ifdef _DEBUG
			char buff[64] = {};
			sprintf_s(buff, "Device Lost on ResizeBuffers: Reason code 0x%08X\n",
				static_cast<unsigned int>((hr == DXGI_ERROR_DEVICE_REMOVED) ? m_d3dDevice->GetDeviceRemovedReason() : hr));
			OutputDebugStringA(buff);
#endif
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			HandleDeviceLost();

			// Everything is set up now. Do not continue execution of this method. HandleDeviceLost will reenter this method
			// and correctly set up the new device.
			return;
		}
		else
		{
			ThrowIfFailed(hr);
		}
	}
	else
	{
		// Create a descriptor for the swap chain.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = backBufferWidth;
		swapChainDesc.Height = backBufferHeight;
		swapChainDesc.Format = backBufferFormat;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = m_backBufferCount;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = (m_options & (c_FlipPresent | c_AllowTearing | c_EnableHDR)) ? DXGI_SWAP_EFFECT_FLIP_DISCARD : DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		swapChainDesc.Flags = (m_options & c_AllowTearing) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
		fsSwapChainDesc.Windowed = TRUE;

		// Create a SwapChain from a Win32 window.
		ThrowIfFailed(m_dxgiFactory->CreateSwapChainForHwnd(
			m_d3dDevice.Get(),
			m_window,
			&swapChainDesc,
			&fsSwapChainDesc,
			nullptr, m_swapChain.ReleaseAndGetAddressOf()
		));

		// This class does not support exclusive full-screen mode and prevents DXGI from responding to the ALT+ENTER shortcut
		ThrowIfFailed(m_dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
	}

	// Handle color space settings for HDR
	UpdateColorSpace();

	// Create a render target view of the swap chain back buffer.
	ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(m_renderTarget.ReleaseAndGetAddressOf())));

	CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc(D3D11_RTV_DIMENSION_TEXTURE2D, m_backBufferFormat);
	ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(
		m_renderTarget.Get(),
		&renderTargetViewDesc,
		m_d3dRenderTargetView.ReleaseAndGetAddressOf()
	));

	if (m_depthBufferFormat != DXGI_FORMAT_UNKNOWN)
	{
		// Create a depth stencil view for use with 3D rendering if needed.
		CD3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = backBufferWidth;
		depthStencilDesc.Height = backBufferHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		ThrowIfFailed(m_d3dDevice->CreateTexture2D(
			&depthStencilDesc,
			nullptr,
			m_depthStencil.ReleaseAndGetAddressOf()
		));

		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D, DXGI_FORMAT_D24_UNORM_S8_UINT);
		ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(
			m_depthStencil.Get(),
			&depthStencilViewDesc,
			m_d3dDepthStencilView.ReleaseAndGetAddressOf()
		));
	}

	// Set the 3D rendering viewport to target the entire window.
	m_screenViewport = CD3D11_VIEWPORT(
		0.0f,
		0.0f,
		static_cast<float>(backBufferWidth),
		static_cast<float>(backBufferHeight)
	);
}

// This method is called when the Win32 window is created (or re-created).
void Xerxes::Engine::SubsystemManagers::Renderer::DeviceResources::SetWindow(HWND window, int width, int height) noexcept
{
	m_window = window;

	m_outputSize.left = m_outputSize.top = 0;
	m_outputSize.right = width;
	m_outputSize.bottom = height;
}

// This method is called when the Win32 window changes size
bool Xerxes::Engine::SubsystemManagers::Renderer::DeviceResources::WindowSizeChanged(int width, int height)
{
	RECT newRc;
	newRc.left = newRc.top = 0;
	newRc.right = width;
	newRc.bottom = height;
	if (newRc == m_outputSize)
	{
		// Handle color space settings for HDR
		UpdateColorSpace();

		return false;
	}

	m_outputSize = newRc;
	CreateWindowSizeDependentResources();
	return true;
}

// Recreate all device resources and set them back to the current state.
void Xerxes::Engine::SubsystemManagers::Renderer::DeviceResources::HandleDeviceLost()
{
	if (m_deviceNotify)
	{
		m_deviceNotify->OnDeviceLost();
	}

	m_d3dDepthStencilView.Reset();
	m_d3dRenderTargetView.Reset();
	m_renderTarget.Reset();
	m_depthStencil.Reset();
	m_swapChain.Reset();
	m_d3dContext.Reset();
	m_d3dAnnotation.Reset();

#ifdef _DEBUG
	{
		ComPtr<ID3D11Debug> d3dDebug;
		if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
		{
			d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
		}
	}
#endif

	m_d3dDevice.Reset();
	m_dxgiFactory.Reset();

	CreateDeviceResources();
	CreateWindowSizeDependentResources();

	if (m_deviceNotify)
	{
		m_deviceNotify->OnDeviceRestored(m_d3dContext.Get(), m_d3dDevice.Get());
	}
}

// Present the contents of the swap chain to the screen.
void Xerxes::Engine::SubsystemManagers::Renderer::DeviceResources::Present()
{
	HRESULT hr = E_FAIL;
	if (m_options & c_AllowTearing)
	{
		// Recommended to always use tearing if supported when using a sync interval of 0.
		hr = m_swapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
	}
	else
	{
		// The first argument instructs DXGI to block until VSync, putting the application
		// to sleep until the next VSync. This ensures we don't waste any cycles rendering
		// frames that will never be displayed to the screen.
		hr = m_swapChain->Present(1, 0);
	}

	// Discard the contents of the render target.
	// This is a valid operation only when the existing contents will be entirely
	// overwritten. If dirty or scroll rects are used, this call should be removed.
	m_d3dContext->DiscardView(m_d3dRenderTargetView.Get());

	if (m_d3dDepthStencilView)
	{
		// Discard the contents of the depth stencil.
		m_d3dContext->DiscardView(m_d3dDepthStencilView.Get());
	}

	// If the device was removed either by a disconnection or a driver upgrade, we
	// must recreate all device resources.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
#ifdef _DEBUG
		char buff[64] = {};
		sprintf_s(buff, "Device Lost on Present: Reason code 0x%08X\n",
			static_cast<unsigned int>((hr == DXGI_ERROR_DEVICE_REMOVED) ? m_d3dDevice->GetDeviceRemovedReason() : hr));
		OutputDebugStringA(buff);
#endif
		HandleDeviceLost();
	}
	else
	{
		ThrowIfFailed(hr);

		if (!m_dxgiFactory->IsCurrent())
		{
			// Output information is cached on the DXGI Factory. If it is stale we need to create a new factory.
			CreateFactory();
		}
	}
}

void Xerxes::Engine::SubsystemManagers::Renderer::DeviceResources::CreateFactory()
{
#if defined(_DEBUG) && (_WIN32_WINNT >= 0x0603 /*_WIN32_WINNT_WINBLUE*/)
	bool debugDXGI = false;
	{
		ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
		{
			debugDXGI = true;

			ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));

			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
			dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);

			DXGI_INFO_QUEUE_MESSAGE_ID hide[] =
			{
				80 /* IDXGISwapChain::GetContainingOutput: The swapchain's adapter does not control the output on which the swapchain's window resides. */,
			};
			DXGI_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = static_cast<UINT>(std::size(hide));
			filter.DenyList.pIDList = hide;
			dxgiInfoQueue->AddStorageFilterEntries(DXGI_DEBUG_DXGI, &filter);
		}
	}

	if (!debugDXGI)
#endif

		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));
}

// This method acquires the first available hardware adapter.
// If no such adapter can be found, *ppAdapter will be set to nullptr.
void Xerxes::Engine::SubsystemManagers::Renderer::DeviceResources::GetHardwareAdapter(IDXGIAdapter1** ppAdapter)
{
	*ppAdapter = nullptr;

	ComPtr<IDXGIAdapter1> adapter;

	ComPtr<IDXGIFactory6> factory6;
	HRESULT hr = m_dxgiFactory.As(&factory6);
	if (SUCCEEDED(hr))
	{
		for (UINT adapterIndex = 0;
			SUCCEEDED(factory6->EnumAdapterByGpuPreference(
				adapterIndex,
				DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
				IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf())));
			adapterIndex++)
		{
			DXGI_ADAPTER_DESC1 desc;
			ThrowIfFailed(adapter->GetDesc1(&desc));

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				continue;
			}

#ifdef _DEBUG
			wchar_t buff[256] = {};
			swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);
			OutputDebugStringW(buff);
#endif

			break;
		}
	}

	if (!adapter)
	{
		for (UINT adapterIndex = 0;
			SUCCEEDED(m_dxgiFactory->EnumAdapters1(
				adapterIndex,
				adapter.ReleaseAndGetAddressOf()));
			adapterIndex++)
		{
			DXGI_ADAPTER_DESC1 desc;
			ThrowIfFailed(adapter->GetDesc1(&desc));

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				continue;
			}

#ifdef _DEBUG
			wchar_t buff[256] = {};
			swprintf_s(buff, L"Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);
			OutputDebugStringW(buff);
#endif

			break;
		}
	}

	*ppAdapter = adapter.Detach();
}

// Sets the color space for the swap chain in order to handle HDR output.
void Xerxes::Engine::SubsystemManagers::Renderer::DeviceResources::UpdateColorSpace()
{
	DXGI_COLOR_SPACE_TYPE colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;

	bool isDisplayHDR10 = false;

	if (m_swapChain)
	{
		ComPtr<IDXGIOutput> output;
		if (SUCCEEDED(m_swapChain->GetContainingOutput(output.GetAddressOf())))
		{
			ComPtr<IDXGIOutput6> output6;
			if (SUCCEEDED(output.As(&output6)))
			{
				DXGI_OUTPUT_DESC1 desc;
				ThrowIfFailed(output6->GetDesc1(&desc));

				if (desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020)
				{
					// Display output is HDR10.
					isDisplayHDR10 = true;
				}
			}
		}
	}

	if ((m_options & c_EnableHDR) && isDisplayHDR10)
	{
		switch (m_backBufferFormat)
		{
		case DXGI_FORMAT_R10G10B10A2_UNORM:
			// The application creates the HDR10 signal.
			colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
			break;

		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			// The system creates the HDR10 signal; application uses linear values.
			colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;
			break;

		default:
			break;
		}
	}

	m_colorSpace = colorSpace;

	ComPtr<IDXGISwapChain3> swapChain3;
	if (SUCCEEDED(m_swapChain.As(&swapChain3)))
	{
		UINT colorSpaceSupport = 0;
		if (SUCCEEDED(swapChain3->CheckColorSpaceSupport(colorSpace, &colorSpaceSupport))
			&& (colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT))
		{
			ThrowIfFailed(swapChain3->SetColorSpace1(colorSpace));
		}
	}
}


void Xerxes::Engine::SubsystemManagers::Renderer::OnInit()
{
	deviceResource = new DeviceResources();
	deviceResource->RegisterDeviceNotify(this);
	renderTarget = std::make_unique<GraphicRenderTarget>();
}

void Xerxes::Engine::SubsystemManagers::Renderer::OnShutdown()
{
	delete deviceResource;
	renderTarget.release();
}

void Xerxes::Engine::SubsystemManagers::Renderer::OnDeviceLost()
{
	for (auto notListener : notifyListeners)
	{
		try
		{
			notListener->OnDeviceLost();
		}
		catch (const std::exception&)
		{

		}
	}
}

void Xerxes::Engine::SubsystemManagers::Renderer::OnDeviceRestored(ID3D11DeviceContext* context, ID3D11Device* device)
{
	renderTarget->Initialize(device, renderWidth, renderHeight);
	for (auto notListener : notifyListeners)
	{
		try
		{
			notListener->OnDeviceRestored(context, device);
		}
		catch (const std::exception&)
		{

		}
	}
}

void Xerxes::Engine::SubsystemManagers::Renderer::SetWindow(HWND window, int width, int height) noexcept
{
	deviceResource->SetWindow(window, width, height);
}

void Xerxes::Engine::SubsystemManagers::Renderer::SetRenderDimansion(int width, int height)
{
	renderTarget->Initialize(deviceResource->GetD3DDevice(), width, height);
	this->renderWidth = width;
	this->renderHeight = height;
}

void Xerxes::Engine::SubsystemManagers::Renderer::GetRenderDimansion(int& width, int& height)
{
	width = this->renderWidth;
	height = this->renderHeight;
}

void Xerxes::Engine::SubsystemManagers::Renderer::Present()
{
	deviceResource->Present();
}

bool Xerxes::Engine::SubsystemManagers::Renderer::SetOutputSize(int width, int height)
{
	return deviceResource->WindowSizeChanged(width, height);
}

void Xerxes::Engine::SubsystemManagers::Renderer::Render()
{
	auto context = deviceResource->GetD3DDeviceContext();
	auto depthStencil = deviceResource->GetDepthStencilView();
	renderTarget->ClearRenderTarget(context, depthStencil, Colors::CornflowerBlue);
	renderTarget->SetRenderTarget(context, depthStencil, deviceResource->GetRenderTargetView());


	deviceResource->PIXBeginEvent(L"Render");
	auto camera = XCameraM()->GetActiveCamera();

	auto view = camera->GetView();
	auto proj = camera->GetProjection();

	auto sceneManager = XSceneM();

	sceneManager->OnRender(view, proj);
	deviceResource->PIXEndEvent();

}

void Xerxes::Engine::SubsystemManagers::Renderer::Clear()
{
	auto context = deviceResource->GetD3DDeviceContext();
	auto renderTarget = deviceResource->GetRenderTargetView();
	auto depthStencil = deviceResource->GetDepthStencilView();

	// Clear the views.
	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	// Set the viewport.
	{
		CD3D11_VIEWPORT viewport(0.f, 0.f, static_cast<float>(renderWidth), static_cast<float>(renderHeight));
		context->RSSetViewports(1, &viewport);
	}
}

void Xerxes::Engine::SubsystemManagers::Renderer::CreateResource()
{
	deviceResource->CreateDeviceResources();
	deviceResource->CreateWindowSizeDependentResources();
	this->OnDeviceRestored(deviceResource->GetD3DDeviceContext(), deviceResource->GetD3DDevice());
}

ID3D11Device* Xerxes::Engine::SubsystemManagers::Renderer::GetDevice()
{
	return deviceResource->GetD3DDevice();
}

ID3D11DeviceContext* Xerxes::Engine::SubsystemManagers::Renderer::GetDeviceContext()
{
	return deviceResource->GetD3DDeviceContext();
}
