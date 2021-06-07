#include "EditorLayer.h"

using namespace HEngine;

EditorLayer::EditorLayer()
    : HEngine::Layer("Editor Layer")
{
    m_Scene = std::make_shared<Scene>();

}
void EditorLayer::OnAttach()
{
    auto testentt = m_Scene->CreateEntity("Test Quad");
    std::vector<FVertex> verts = {
        {Vec3{ -0.5f, -0.5f, 0.0f }, Vec4{ 0.1f, 0.1f,0.8f,1.0f }},
        { Vec3{0.5f, -0.5f, 0.0f}, Vec4{0.1f, 0.1f,0.8f,1.0f} },
        { Vec3{0.5f, 0.5f, 0.0f} , Vec4{0.1f, 0.1f,0.8f,1.0f} },
        { Vec3{-0.5f, 0.5f, 0.0f}, Vec4{0.1f, 0.1f,0.8f,1.0f} },
    };
    std::vector<uint32_t> indx = { 0, 1, 2, 2, 3, 0 };
    testentt.AddComponent<MeshRendererComponent>(verts, indx);

    EditorCamera = m_Scene->CreateEntity("Editor Camera");
    EditorCamera.AddComponent<CameraComponent>();
    EditorCamera.GetComponent<CameraComponent>().bPrimary = true;
    m_OutlinerPanel.SetContext(m_Scene);

    auto main_Shader = new Shader("assets/shaders/simpleShader.glsl");
    testentt.GetComponent<MeshRendererComponent>().shader = main_Shader;

}

void EditorLayer::OnUpdate()
{
    HEngine::Renderer::prepareScene(m_Scene.get());
    HEngine::Renderer::submitScene(m_Scene.get());
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
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows, 
                // which we can't undo at the moment without finer window depth/z control.
                //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                if (ImGui::MenuItem("Exit")) HEngine::Application::Get().Close();
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }
        ImGui::End();
    }
        m_OutlinerPanel.OnImGuiRender();
}

void EditorLayer::OnEvent(HEngine::Event& event)
{

}

