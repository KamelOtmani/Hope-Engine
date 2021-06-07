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
        { Vec3{0.5f, -0.5f, 0.0f},   Vec4{0.8f, 0.1f,0.8f,1.0f} },
        { Vec3{0.5f, 0.5f, 0.0f} ,   Vec4{0.8f, 0.1f,0.8f,1.0f} },
        { Vec3{-0.5f, 0.5f, 0.0f},   Vec4{0.8f, 0.1f,0.8f,1.0f} },
    };
    std::vector<uint32_t> indx = { 0, 1, 2, 2, 3, 0 };
    testentt.AddComponent<MeshRendererComponent>(verts, indx);

    EditorCamera = m_Scene->CreateEntity("Editor Camera");
    EditorCamera.AddComponent<CameraComponent>();
    EditorCamera.GetComponent<TransformComponent>().Position = Vec3{ 0.0f,0.0f,3.0f };
    EditorCamera.GetComponent<CameraComponent>().bPrimary = true;
    m_OutlinerPanel.SetContext(m_Scene);

    auto main_Shader = new Shader("assets/shaders/simpleShader.glsl");
    testentt.GetComponent<MeshRendererComponent>().shader = main_Shader;

    defaultTexture = HEngine::Texture2D::Create("assets/textures/test.jpg");

    HEngine::FramebufferSpecification fbspec;
    fbspec.width = 1600;
    fbspec.height = 900;

    m_MainFramebuffer = HEngine::Framebuffer::Create(fbspec);
}

void EditorLayer::OnUpdate()
{
    m_MainFramebuffer->Bind();
    HEngine::Renderer::prepareScene(m_Scene.get());
    HEngine::Renderer::submitScene(m_Scene.get());
    m_MainFramebuffer->UnBind();
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
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Main Viewport");
    m_ViewportFocused = ImGui::IsWindowFocused();
    m_ViewportHovered = ImGui::IsWindowHovered();
    Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

    auto ViewportPanelSize = ImGui::GetContentRegionAvail();
    if ((m_ViewportHeight != ViewportPanelSize.y) | (m_ViewportWidth != ViewportPanelSize.x))
    {
        m_ViewportHeight = ViewportPanelSize.y;
        m_ViewportWidth = ViewportPanelSize.x;
        m_MainFramebuffer->Resize(m_ViewportWidth, m_ViewportHeight);
        EditorCamera.GetComponent<CameraComponent>().aspectRatio = (float)m_ViewportWidth / (float)m_ViewportHeight;
    }
    auto textID = m_MainFramebuffer->getColorAttachement();
    ImGui::Image((void*)textID, ViewportPanelSize, ImVec2{ 0,1 }, ImVec2{ 1,0 });
    ImGui::End();
    ImGui::PopStyleVar();
    m_OutlinerPanel.OnImGuiRender();
}

void EditorLayer::OnEvent(HEngine::Event& event)
{

}

