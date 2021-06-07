#pragma once
#include <HEngine.h>
#include <imgui.h>

#include "Panels/OutlinerPanel.h"

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
    HEngine::Entity m_SelectionContext;
    OutlinerPanel m_OutlinerPanel;
    Ref<HEngine::Texture2D> defaultTexture;
};