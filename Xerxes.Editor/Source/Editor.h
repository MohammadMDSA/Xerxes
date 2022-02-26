//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "EditorWindow.h"

//class EditorWindow;

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Editor
{
public:

    Editor() noexcept;
    ~Editor() = default;

    Editor(Editor&&) = default;
    Editor& operator= (Editor&&) = default;

    Editor(Editor const&) = delete;
    Editor& operator= (Editor const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();
    void PostResourceCreation();

    void OnDeviceLost();

    void InitializeImgui();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext;

    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;
    bool                                            m_imguiActive;

    bool                                            show_demo_window;
    bool                                            show_another_window;
    float                                           clear_color[3];

    DirectX::SimpleMath::Matrix m_world;
    DirectX::SimpleMath::Matrix m_view;
    DirectX::SimpleMath::Matrix m_proj;

    DirectX::SimpleMath::Vector3 position;

    std::unique_ptr<DirectX::GeometricPrimitive>    m_shape;

    EditorWindow*                                   sceneWindow;
};
