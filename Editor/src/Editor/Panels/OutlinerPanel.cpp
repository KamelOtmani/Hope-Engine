#include "OutlinerPanel.h"

#include <imgui.h>
#include "ECS\Components.h"

#include <glm/gtc/type_ptr.hpp>

using namespace HEngine;

OutlinerPanel::OutlinerPanel(const Ref<Scene>& context)
{
    SetContext(context);
}

void OutlinerPanel::SetContext(const Ref<Scene>& context)
{
    m_Scene = context;
}

void OutlinerPanel::OnImGuiRender()
{
    ImGui::Begin("Outliner");

    auto& view = m_Scene->m_Registry.view<TagComponent>();
    for (auto& entity : view)
    {
        Entity entt{ entity, m_Scene.get() };
        DrawEntityNode(entt);
    }


    ImGui::End();

    ImGui::Begin("Properties");
    if (m_SelectionContext)
    {
        DrawComponent(m_SelectionContext);
    }
    ImGui::End();
    ImGui::Begin("LOL");
    ImGui::End();
}

void OutlinerPanel::DrawEntityNode(Entity entity)
{
    auto& tag = entity.GetComponent<TagComponent>().tag;

    ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
    if (ImGui::IsItemClicked())
    {
        m_SelectionContext = entity;
    };

    if (opened)
    {
        ImGui::TreePop();
    }

}

void OutlinerPanel::DrawComponent(HEngine::Entity entity)
{
    if (entity.HasComponent<TagComponent>())
    {
        auto& tag = entity.GetComponent<TagComponent>().tag;

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), tag.c_str());
        if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
        {
            tag = std::string(buffer);
        }
    }

    if (entity.HasComponent<TransformComponent>())
    {
        if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
        {
            auto& transform = entity.GetComponent<TransformComponent>();
            ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.1f);
            ImGui::DragFloat3("Rotation", glm::value_ptr(transform.Rotation), 0.1f);
            ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.1f);

            ImGui::TreePop();
        }
    }
}
