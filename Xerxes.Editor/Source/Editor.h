//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "GameObject.h"
#include "SceneWindow.h"
#include "InspectorWindow.h"
#include "RootManager.h"
#include "HierarchyWindow.h"
#include "ResourceWindow.h"
#include "GraphicRenderTarget.h"

using namespace DX;
using namespace Xerxes::Engine::Graphics::Device;

namespace Xerxes
{
    namespace Editor
    {
        //class EditorWindow;

        // A basic game implementation that creates a D3D11 device and
        // provides a game loop.
        class Editor final : public IDeviceNotify
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
            void OnDeviceRestored(ID3D11DeviceContext* context, ID3D11Device* device) override;

            // Messages
            void OnActivated();
            void OnDeactivated();
            void OnSuspending();
            void OnResuming();
            void OnWindowMoved();
            void OnWindowSizeChanged(int width, int height);

            // Properties
            void GetDefaultSize(int& width, int& height) const noexcept;

        private:

            void Update(StepTimer const& timer);
            void Render();
            void RenderGUI();

            void CreateDeviceDependentResources();
            void CreateWindowSizeDependentResources();

            void InitializeImgui();
            void AppBarMenus();
            void AddItem();

            void MakeDockSpace();

            // Device resources.
            HWND                                            m_window;
            int                                             m_outputWidth;
            int                                             m_outputHeight;

            // Rendering loop timer.
            StepTimer                                       m_timer;
            bool                                            m_imguiActive;

            bool                                            showDemo;
            bool                                            showCameraInspector;

            float                                           clear_color[3];

            RootManager* rootManager;

            Panels::SceneWindow* sceneWindow;
            Panels::InspectorWindow* inspectorWindow;
            Panels::HierarchyWindow* hierarchyWindow;
            Panels::ResourceWindow* resourceWindow;

        };
    }
}