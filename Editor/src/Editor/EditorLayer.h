#pragma once
#include <HEngine.h>
#include <imgui.h>

#include "Panels/OutlinerPanel.h"
#include "Renderer/Framebuffer.h"

class EditorLayer : public HEngine::Layer
{
public:
    EditorLayer();
    virtual ~EditorLayer() = default;

    void OnAttach() override;

    void OnUpdate() override;
    void OnImGuiRender() override;

    void OnEvent(HEngine::Event& event) override;
private:
    Ref<HEngine::Scene> m_Scene;
    HEngine::Entity EditorCamera;

    // Rendering resources
    Ref<HEngine::Framebuffer> m_MainFramebuffer;

    HEngine::Entity m_SelectionContext;
    
    // UI
    OutlinerPanel m_OutlinerPanel;
    uint32_t m_ViewportHeight, m_ViewportWidth;
    bool m_ViewportFocused = false, m_ViewportHovered = false;

    // Default resources
    Ref<HEngine::Texture2D> defaultTexture;
};