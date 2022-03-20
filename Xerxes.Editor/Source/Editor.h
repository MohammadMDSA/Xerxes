//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "GameObject.h"
#include "SceneWindow.h"
#include "InspectorWindow.h"
#include "RootManager.h"
#include "DeviceResources.h"
#include "HierarchyWindow.h"
#include "ResourceWindow.h"
#include "EditorWindowGraphicResource.h"

//class EditorWindow;

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Editor final : public DX::IDeviceNotify
{
public:

    Editor() noexcept;
    ~Editor();

    Editor(Editor&&) = default;
    Editor& operator= (Editor&&) = default;

    Editor(Editor const&) = delete;
    Editor& operator= (Editor const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    
    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    void InitializeImgui();
    void AppBarMenus();
    void AddItem();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;
    int                                             sceneWidth;
    int                                             sceneHeight;

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;
    bool                                            m_imguiActive;

    bool                                            showDemo;
    bool                                            showCameraInspector;

    float                                           clear_color[3];

    RootManager*                                    rootManager;

    SceneWindow*                                    sceneWindow;
    InspectorWindow*                                inspectorWindow;
    HierarchyWindow*                                hierarchyWindow;
    ResourceWindow*                                 resourceWindow;

    EditorWindowGraphicResource*                    windowResource;

};
