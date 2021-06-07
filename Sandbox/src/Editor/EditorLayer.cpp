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
    m_OutlinerPanel.OnImGuiRender();

}

void EditorLayer::OnEvent(HEngine::Event& event)
{

}

