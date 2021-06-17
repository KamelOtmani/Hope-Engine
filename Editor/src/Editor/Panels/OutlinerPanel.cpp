#include "OutlinerPanel.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "ECS\Components.h"
#include "Utility\PlatformUtils.h"

#include <glm/gtc/type_ptr.hpp>

using namespace HEngine;

OutlinerPanel::OutlinerPanel(const Ref<Scene>& context)
{
    SetContext(context);
}

void OutlinerPanel::SetContext(const Ref<Scene>& context)
{
    m_Scene = context;
    m_SelectionContext = {};
}

HEngine::Entity OutlinerPanel::GetSelectedEntity() const
{
     return m_SelectionContext; 
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
    // Right-click on blank space
    if (ImGui::BeginPopupContextWindow(0, 1, false))
    {
        if (ImGui::MenuItem("Create Empty Entity"))
            m_Scene->CreateEntity("Empty Entity");

        ImGui::EndPopup();
    }

    ImGui::End();

    ImGui::Begin("Properties");

    if (m_SelectionContext)
    {
        DrawComponents(m_SelectionContext);
    }
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

static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
    ImGui::PushID(label.c_str());
    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 1, 1 });

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight/1.25f , lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
    if (ImGui::Button("X", buttonSize))
        values.x = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
    if (ImGui::Button("Y", buttonSize))
        values.y = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
    if (ImGui::Button("Z", buttonSize))
        values.z = resetValue;
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();
    ImGui::PopID();
}

template<typename T, typename UIFunction>
static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
{
    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
    if (entity.HasComponent<T>())
    {
        auto& component = entity.GetComponent<T>();
        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
        ImGui::PopStyleVar(
        );
        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
        if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
        {
            ImGui::OpenPopup("ComponentSettings");
        }

        bool removeComponent = false;
        if (ImGui::BeginPopup("ComponentSettings"))
        {
            if (ImGui::MenuItem("Remove component"))
                removeComponent = true;

            ImGui::EndPopup();
        }

        if (open)
        {
            uiFunction(component);
            ImGui::TreePop();
        }

        if (removeComponent)
            entity.RemoveComponent<T>();
    }
}

void OutlinerPanel::DrawComponents(HEngine::Entity entity)
{
    if (entity.HasComponent<TagComponent>())
    {

        auto& tag = entity.GetComponent<TagComponent>().tag;

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), tag.c_str());
        if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
        {
            tag = std::string(buffer);
        }
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(-1);

    if (ImGui::Button("Add Component", ImVec2{ -1.0,25.f }))
        ImGui::OpenPopup("AddComponent");

    if (ImGui::BeginPopup("AddComponent"))
    {
        if (!m_SelectionContext.HasComponent<CameraComponent>())
        {
            if (ImGui::MenuItem("Camera Component"))
            {
                m_SelectionContext.AddComponent<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }
        }
        
        if (!m_SelectionContext.HasComponent<SpriteRendererComponent>())
        {
            if (ImGui::MenuItem("Sprite Component"))
            {

                m_SelectionContext.AddComponent<SpriteRendererComponent>();
                ImGui::CloseCurrentPopup();
            }

        }

        if (!m_SelectionContext.HasComponent<MeshRendererComponent>())
        {
            if (ImGui::MenuItem("Mesh Component"))
            {

                m_SelectionContext.AddComponent<MeshRendererComponent>();
                m_SelectionContext.GetComponent<MeshRendererComponent>().material = m_Scene->m_DefaultMaterial;
                ImGui::CloseCurrentPopup();
            }
        }
        if (!m_SelectionContext.HasComponent<PointLightComponent>())
        {
            if (ImGui::MenuItem("Point Light Component"))
            {

                m_SelectionContext.AddComponent<PointLightComponent>();
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
    ImGui::PopItemWidth();

    DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
        {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
        ImGui::Columns(2);
        //ImGui::SetColumnWidth(0, 90);
        ImGui::Separator();

        int obj_i = 0;
        ImGui::PushID((obj_i + 5231433) * 2);
        ImGui::AlignTextToFramePadding();   // Text and Tree nodes are less high than framed widgets, here we add vertical spacing to make the tree lines equal high.
        ImGui::Text("Translation");
        ImGui::NextColumn();
        ImGui::AlignTextToFramePadding();
        DrawVec3Control("Translation", component.Position);
        ImGui::NextColumn();
        ImGui::PopID();

        ImGui::PushID((obj_i + 5231433) * 2);
        ImGui::AlignTextToFramePadding();   // Text and Tree nodes are less high than framed widgets, here we add vertical spacing to make the tree lines equal high.
        ImGui::Text("Rotation");
        ImGui::NextColumn();
        ImGui::AlignTextToFramePadding();
        DrawVec3Control("Rotation", component.Rotation);
        ImGui::NextColumn();
        ImGui::PopID();

        ImGui::PushID((obj_i + 5231433) * 2);
        ImGui::AlignTextToFramePadding();   // Text and Tree nodes are less high than framed widgets, here we add vertical spacing to make the tree lines equal high.
        ImGui::Text("Scale");
        ImGui::NextColumn();
        ImGui::AlignTextToFramePadding();
        DrawVec3Control("Scale", component.Scale, 1.0f);
        ImGui::NextColumn();
        ImGui::PopID();


        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::PopStyleVar();
        });

    DrawComponent<CameraComponent>("Camera Component", entity, [](auto& component)
        {
            auto& cam = component;

            const char* projString[] = { "Perspective","Orthographic" };
            const char* currentProjString = projString[(int)cam.projType];
            if (ImGui::BeginCombo("Projection", currentProjString))
            {
                for (int i = 0; i < 2; i++)
                {
                    bool bSelected = projString[i] == currentProjString;
                    if (ImGui::Selectable(projString[i], bSelected))
                    {
                        currentProjString = projString[i];
                        cam.projType = (CameraProjectionType)i;
                    }
                    if (bSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            if (cam.projType == CameraProjectionType::Perspective)
            {
                ImGui::DragFloat("FOV", &cam.fov, 1.0f, 0.0f, 180.f);
                ImGui::DragFloat("Near Plane", &cam.nearPlane, 1.0f, 0.0f, 10000.f);
                ImGui::DragFloat("Far Plane", &cam.farPlane, 1.0f, 0.0f, 10000.f);
            }
            if (cam.projType == CameraProjectionType::Orthographic)
            {
                ImGui::DragFloat("Size", &cam.orthographicSize, 1.0f, 0.0f, 100.f);
                ImGui::DragFloat("Near Plane", &cam.orthographicNearPlane, 1.0f, 0.0f, 10000.f);
                ImGui::DragFloat("Far Plane", &cam.orthographicFarPlane, 1.0f, 0.0f, 10000.f);
            }
            ImGui::Checkbox("Primary Camera", &cam.bPrimary);
        });

    DrawComponent<SpriteRendererComponent>("Sprite Renderer Component", entity, [](auto& component)
        {
            auto& sprite = component;
            ImGui::ColorEdit4("Color", glm::value_ptr(sprite.m_Color));
        });

    DrawComponent<PointLightComponent>("Point Light Component", entity, [](auto& component)
        {
            auto& light = component;
            ImGui::DragFloat("Intensity", &light.m_Intensity);
            ImGui::ColorEdit4("Color", glm::value_ptr(light.m_Color));
            ImGui::Checkbox("Affect World", &light.bAffectWorld);
        });

    DrawComponent<MeshRendererComponent>("Mesh Renderer Component", entity, [this](auto& component)
        {
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), component.path.c_str());
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                component.path = std::string(buffer);
            }
            ImGui::SameLine();
            if (ImGui::Button("..."))
            {
                component.path = FileDialogs::OpenFile("Wavefront (*.obj)\0*.obj\0FBX (*.fbx)\0*.fbx\0");
                if (!component.path.empty())
                    component.UpdateMesh();
            }
            if (component.material)
            {
                if (ImGui::BeginCombo("##Material", component.material->name.c_str()))
                {
                    for (const auto& mat : m_Scene->MatLibrary.Library)
                    {
                        ImGui::PushID(mat->name.size() + (int*)mat.get());
                        if (ImGui::Selectable(mat->name.c_str(), mat == component.material))
                            component.material = mat;
                        ImGui::PopID();
                    }
                    ImGui::EndCombo();
                }
            }
        });

}
