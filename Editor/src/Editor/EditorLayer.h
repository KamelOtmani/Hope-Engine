#pragma once
#include <HEngine.h>
#include <imgui.h>

#include "Renderer/EditorCamera.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/SceneRenderer.h"

#include "Panels/OutlinerPanel.h"
#include "Panels/ContentBrowser.h"

class EditorLayer : public HEngine::Layer {
public:
    EditorLayer();
    virtual ~EditorLayer() = default;

    void OnAttach() override;

    void OnUpdate(float dt) override;
    void OnImGuiRender() override;

    void OnEvent(HEngine::Event& event) override;

private:
    bool OnKeyPressed(HEngine::KeyPressedEvent& e);

    void NewScene();
    void OpenScene();
    void SaveSceneAs();
    void ImportMeshes(float scale);

private:
    Ref<HEngine::Scene> m_Scene;
    HEngine::SceneRenderer m_SceneRenderer;
    HEngine::EditorCamera m_EditorCamera;

    // Rendering resources
    Ref<HEngine::Framebuffer> m_MainFramebuffer;

    HEngine::Entity m_SelectionContext;
    float FrameTime = 0;

    // UI //
    // Pannels
    OutlinerPanel m_OutlinerPanel;
    ContentBrowserPannel m_ContentBrowserPannel;
    // Settings
    uint32_t m_ViewportHeight, m_ViewportWidth;
    bool m_ViewportFocused = false, m_ViewportHovered = false;
    int m_GizmoType = -1;
    float ImprotScale = 1;
};
