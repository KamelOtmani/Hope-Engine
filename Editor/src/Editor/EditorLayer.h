#pragma once
#include <HEngine.h>
#include <imgui.h>

#include "Panels/OutlinerPanel.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/EditorCamera.h"

class EditorLayer : public HEngine::Layer
{
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
    HEngine::EditorCamera m_EditorCamera;

    // Rendering resources
    Ref<HEngine::Framebuffer> m_MainFramebuffer;

    HEngine::Entity m_SelectionContext;
    
    // UI
    OutlinerPanel m_OutlinerPanel;
    uint32_t m_ViewportHeight, m_ViewportWidth;
    bool m_ViewportFocused = false, m_ViewportHovered = false;
    int m_GizmoType = -1;
    float ImprotScale;

};