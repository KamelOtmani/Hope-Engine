#include <HEngine.h>
#include <imgui.h>
#include <Core/EntryPoint.h>



using namespace HEngine;

class ExampleLayer : public HEngine::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
        m_Scene = std::make_shared<Scene>();

    }
    void OnAttach() override
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

        auto cam = m_Scene->CreateEntity("Camera");
        cam.AddComponent<CameraComponent>();
        cam.GetComponent<CameraComponent>().bPrimary = true;

        auto main_Shader = new Shader("assets/shaders/simpleShader.glsl");
        testentt.GetComponent<MeshRendererComponent>().shader = main_Shader;

    }

    void OnUpdate() override
    {
        HEngine::Renderer::prepareScene(m_Scene.get());
        HEngine::Renderer::submitScene(m_Scene.get());
    }
    void OnImGuiRender() override
    {

        ImGui::Begin("Outliner");

        auto& view = m_Scene->m_Registry.view<TagComponent>();
        for (auto& entity : view)
        {
            Entity entt{ entity, m_Scene.get() };
            auto& tag = entt.GetComponent<TagComponent>().tag;

            ImGuiTreeNodeFlags flags =  ImGuiTreeNodeFlags_OpenOnArrow;
            bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

            if (opened)
            {
                ImGui::TreePop();
            }
        }
        ImGui::End();
        
    }

    void OnEvent(HEngine::Event& event) override
    {

    }

    Ref<HEngine::Scene> m_Scene;

};

class SandboxApp : public HEngine::Application
{
public:
    SandboxApp();
    ~SandboxApp();

private:

};

SandboxApp::SandboxApp()
{
    PushLayer(new ExampleLayer());
}

SandboxApp::~SandboxApp()
{
}

HEngine::Application* HEngine::CreateApplication()
{
    return new SandboxApp();

}