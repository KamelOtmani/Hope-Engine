#pragma once

#include "ECS/Entity.h"
#include "ECS/Entity.h"


class OutlinerPanel
{
public:
    OutlinerPanel() = default;
    OutlinerPanel(const std::shared_ptr<HEngine::Scene>& scene);

    void SetContext(const std::shared_ptr<HEngine::Scene>& scene);

    void OnImGuiRender();
private:
    void DrawEntityNode(HEngine::Entity entity);
    void DrawComponent(HEngine::Entity entity);
private:
    std::shared_ptr<HEngine::Scene> m_Scene;
    HEngine::Entity m_SelectionContext;
};