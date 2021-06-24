#include "EditorLayer.h"
#include "HEngine.h"

#include "ECS/Serializer.h"
#include "Utility/PlatformUtils.h"

#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include "Maths/Maths.h"

using namespace HEngine;

EditorLayer::EditorLayer()
    : HEngine::Layer("Editor Layer")
{
    m_Scene = std::make_shared<Scene>();
}
void EditorLayer::OnAttach()
{

    m_EditorCamera = EditorCamera(60.0f, 1.778f, 0.1f, 1000.0f);

    auto testentt = m_Scene->CreateEntity("Test mesh");
    std::vector<FVertex> verts = {
            { Vec3{-0.5f, -0.5f, 0.0f }  ,Vec3{0.0f,0.0f,1.0f }, Vec2{0.0f,0.0f},  Vec4{1.0f}},
            { Vec3{0.5f, -0.5f, 0.0f}    ,Vec3{0.0f,0.0f,1.0f }, Vec2{1.0f,0.0f},  Vec4{1.0f}},
            { Vec3{0.5f, 0.5f, 0.0f}     ,Vec3{0.0f,0.0f,1.0f }, Vec2{1.0f,1.0f},  Vec4{1.0f}},
            { Vec3{-0.5f, 0.5f, 0.0f}    ,Vec3{0.0f,0.0f,1.0f }, Vec2{0.0f,1.0f},  Vec4{1.0f}}
    };
    std::vector<uint32_t> indx = { 0, 1, 2, 2, 3, 0 };
    testentt.AddComponent<MeshRendererComponent>(verts, indx);

    m_OutlinerPanel.SetContext(m_Scene);

    auto& sprite = m_Scene->CreateEntity("Test Sprite");
    sprite.AddComponent<SpriteRendererComponent>(Vec4{ 0.5f });


    auto& light = m_Scene->CreateEntity("Point light");
    light.AddComponent<PointLightComponent>();

    testentt.GetComponent<MeshRendererComponent>().material = m_Scene->m_DefaultMaterial;
    testentt.GetComponent<MeshRendererComponent>().path = "assets/meshs/Sphere_1m.obj";
    testentt.GetComponent<MeshRendererComponent>().UpdateMesh();
    m_EditorCamera.SetViewportSize(1600, 900);
    m_SceneRenderer.Initialize();
    m_SceneRenderer.m_Scene = m_Scene;
}

void EditorLayer::OnUpdate(float dt)
{
    // Update the camera  
    m_EditorCamera.OnUpdate(dt);
    m_SceneRenderer.m_Scene = m_Scene;
    m_SceneRenderer.Update(m_EditorCamera,dt);
}
void EditorLayer::OnImGuiRender()
{
    // Note: Switch this to true to enable dockspace
    static bool dockingEnabled = true;
    if (dockingEnabled)
    {
        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        style.WindowMinSize.x = minWinSizeX;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Disabling full screen would allow the window to be moved to the front of other windows, 
                // which we can't undo at the moment without finer window depth/z control.
                //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                if (ImGui::MenuItem("New Scene", "Ctrl+N"))
                {
                    NewScene();
                }

                if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
                {
                    OpenScene();
                }


                if (ImGui::MenuItem("Save Scene As", "Ctrl+S"))
                {
                    SaveSceneAs();
                }
                ImGui::Separator();
                ImGui::SetNextItemWidth(150.f);
                ImGui::DragFloat("Import Scale", &ImprotScale, 0.1f);
                if (ImGui::MenuItem("Import scene from model", "Ctrl+O"))
                {
                    ImportMeshes(ImprotScale);
                }
                ImGui::Separator();

                if (ImGui::MenuItem("Exit"))
                    HEngine::Application::Get().Close();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        ImGui::End();
    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Main Viewport");
    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

    auto ViewportPanelSize = ImGui::GetContentRegionAvail();
    if ((m_ViewportHeight != ViewportPanelSize.y) | (m_ViewportWidth != ViewportPanelSize.x))
    {
        m_ViewportHeight = static_cast<uint32_t>(ViewportPanelSize.y);
        m_ViewportWidth = static_cast<uint32_t>(ViewportPanelSize.x);
        //m_MainFramebuffer->Resize(m_ViewportWidth, m_ViewportHeight);
        m_SceneRenderer.OnViewportResize(m_ViewportWidth, m_ViewportHeight);
        m_EditorCamera.SetViewportSize((float)m_ViewportWidth, (float)m_ViewportHeight);
    }
    auto textID = m_SceneRenderer.getOutputTextureID();
    ImGui::Image((void*)textID, ViewportPanelSize, ImVec2{ 0,1 }, ImVec2{ 1,0 });

    // GUIZMOS
    Entity selectedEntity = m_OutlinerPanel.GetSelectedEntity();

    if (selectedEntity && m_GizmoType != -1)
    {
        ImGuizmo::SetOrthographic("false");
        ImGuizmo::SetDrawlist();
        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

        //// camera
        //auto& cam = EditorCamera.GetComponent<CameraComponent>();
        //Mat4 cameraView = glm::inverse(EditorCamera.GetComponent<TransformComponent>().Matrix());
        //Mat4 cameraProjection = cam.Projection();

        // Editor camera
        const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();
        glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

        // Entity
        auto& tc = selectedEntity.GetComponent<TransformComponent>();
        Mat4 transform = tc.Matrix();

        // Snapping
        bool snap = Input::IsKeyPressed(Key::LeftControl);
        float snapValue = 0.5f; // Snap to 0.5m for translation/scale
        // Snap to 45 degrees for rotation
        if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
            snapValue = 45.0f;

        float snapValues[3] = { snapValue, snapValue, snapValue };
        bool bBounds = false;

        ImGuizmo::OPERATION op;

        switch (m_GizmoType)
        {
        case 0:
            op = ImGuizmo::TRANSLATE; break;
        case 1:
            op = ImGuizmo::ROTATE; break;
        case 2:
            op = ImGuizmo::SCALE; break;
        case 3:
            op = ImGuizmo::BOUNDS; break;
            bBounds = true;
        default:
            break;
        }

        static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };

        //ImGuizmo::DrawGrid(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), glm::value_ptr(Mat4(1.0f)), 10.f);

        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
            op, ImGuizmo::MODE::LOCAL, glm::value_ptr(transform),
            nullptr, snap ? snapValues : nullptr, bBounds ? bounds : NULL);

        if (ImGuizmo::IsUsing())
        {
            Vec3 Translation, Rotation, Scale;
            Math::DecomposeTransform(transform, Translation, Rotation, Scale);

            Vec3 DeltaRotation = Rotation - tc.Rotation;
            tc.Position = Translation;
            tc.Rotation += DeltaRotation;
            tc.Scale = Scale;
        }
    }

    ImGui::End();
    ImGui::PopStyleVar();
    m_OutlinerPanel.OnImGuiRender();

    // Material editor
    ImGui::Begin("Material editor");

    if (ImGui::Button("Add new Material"))
    {
        m_Scene->MatLibrary.AddNewMaterial(std::string("UNNAMED MATERIAL"));
        m_Scene->MatLibrary.Library.back()->shader = m_Scene->m_DefaultShader;
    }
    auto& mats = m_Scene->MatLibrary.Library;

    for (auto& mat : mats)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

        if (ImGui::TreeNodeEx((void*)mat.get(), flags, mat->name.c_str()))
        {
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), mat->name.c_str());
            if (ImGui::InputText("Name", buffer, sizeof(buffer)))
            {
                mat->name = std::string(buffer);
            }
            ImGui::ColorEdit4("Color", glm::value_ptr(mat->Color));
            ImGui::DragFloat("Roughness", &mat->Roughness,0.05f,0.0f,1.0f);
            {
                ImGui::Text("Diffuse Texture");
                ImGui::SameLine();
                if (mat->m_AlbedoTexture)
                {
                    ImGui::Text("ID = %d", mat->m_AlbedoTexture->getID());
                }
                ImGui::PushID(45);
                if (ImGui::Button("..."))
                {
                    auto& path = FileDialogs::OpenFile("PNG (*.png)\0*.png\0");
                    if (!path.empty())
                        mat->m_AlbedoTexture = Texture2D::Create(path);
                }
                ImGui::PopID();
            }
            {
                ImGui::Text("Roughness Texture");
                ImGui::SameLine();
                if (mat->m_RoughnessTexture)
                {
                    ImGui::Text("ID = %d", mat->m_RoughnessTexture->getID());
                }
                ImGui::PushID(48);
                if (ImGui::Button("..."))
                {
                    auto& path = FileDialogs::OpenFile("PNG (*.png)\0*.png\0");
                    if (!path.empty())
                        mat->m_RoughnessTexture = Texture2D::Create(path);
                }
                ImGui::PopID();
            }
            ImGui::DragFloat("Metalic", &mat->Metalic, 0.05f, 0.0f, 1.0f);

            ImGui::TreePop();
        }
    }

    ImGui::End();

    ImGui::Begin("Debug");
    // Using the _simplified_ one-liner Combo() api here
    // See "Combo" section for examples of how to use the more complete BeginCombo()/EndCombo() api.
    for (auto& kv : m_SceneRenderer.DebugFBTexturesID)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (ImGui::TreeNodeEx((void*)kv.second, flags, kv.first.c_str()))
        {
            ImGui::Image((void*)kv.second, ViewportPanelSize, ImVec2{ 0,1 }, ImVec2{ 1,0 });

            ImGui::TreePop();
        }
    }
    //ImGui::DragFloat("SSAO Radius", &m_SceneRenderer.radius);
    //ImGui::DragFloat("SSAO Scale", &m_SceneRenderer.scale);
    //ImGui::DragFloat("SSAO Bias", &m_SceneRenderer.bias);
    //ImGui::DragFloat("SSAO Power", &m_SceneRenderer.power);
    ImGui::End();
}

void EditorLayer::OnEvent(HEngine::Event& event)
{
    m_EditorCamera.OnEvent(event);
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(HBIND_EVENT(EditorLayer::OnKeyPressed));
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
{
    // Shortcuts
    if (e.GetRepeatCount() > 0)
        return false;

    bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
    bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
    switch (e.GetKeyCode())
    {
    case Key::N:
    {
        if (control)
            NewScene();

        break;
    }
    case Key::O:
    {
        if (control)
            OpenScene();

        break;
    }
    case Key::S:
    {
        if (control && shift)
            SaveSceneAs();

        break;
    }
    case Key::Space:
    {
        m_GizmoType = (m_GizmoType + 1)%4;
        HLOG("Type = {0}", m_GizmoType);
        break;
    }
    case Key::Q:
    {
        m_GizmoType = -1;
        break;
    }
    case Key::F:
    {
        if (m_OutlinerPanel.m_SelectionContext)
        {
            m_EditorCamera.SetFocus(m_OutlinerPanel.m_SelectionContext.GetComponent<TransformComponent>().Position);
        }
        break;
    }
    }
    return true;
}

void EditorLayer::NewScene()
{
    m_Scene = CreateRef<Scene>();
    m_OutlinerPanel.SetContext(m_Scene);
}

void EditorLayer::OpenScene()
{
    std::string filepath = FileDialogs::OpenFile("Hope Scene (*.hpscene)\0*.hpscene\0");
    if (!filepath.empty())
    {
        m_Scene = CreateRef<Scene>();
        m_OutlinerPanel.SetContext(m_Scene);

        Serializer serializer(m_Scene);
        serializer.Deserialize(filepath);
    }
}

void EditorLayer::SaveSceneAs()
{
    std::string filepath = FileDialogs::SaveFile("Hope Scene (*.hpscene)\0*.hpscene\0");
    if (!filepath.empty())
    {
        Serializer serializer(m_Scene);
        serializer.Serialize(filepath);
    }
}

void EditorLayer::ImportMeshes(float scale)
{
    std::string filepath = FileDialogs::OpenFile("Hope Scene (*.hpscene)\0*.hpscene\0");
    if (!filepath.empty())
    {
        AssetImporter::ImportScene(filepath, m_Scene.get(),scale);
    }
}
