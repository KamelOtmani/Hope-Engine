#pragma once
#include "HEngine.h"
#include "ECS/Entity.h"
#include "ECS/Scene.h"


class OutlinerPanel
{
public:
    OutlinerPanel() = default;
    OutlinerPanel(const Ref<HEngine::Scene>& scene);

    void SetContext(const Ref<HEngine::Scene>& scene);
    HEngine::Entity GetSelectedEntity() const;

    HEngine::Entity m_SelectionContext;
    void OnImGuiRender();
private:
    void DrawEntityNode(HEngine::Entity entity);
    void DrawComponents(HEngine::Entity entity);
private:
    Ref<HEngine::Scene> m_Scene;
};