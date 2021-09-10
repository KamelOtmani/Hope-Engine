#include "hpch.h"
#include "Scene.h"
#include "ECS\Components.h"
#include "ECS\Entity.h"

#include "Renderer/Core/VertexArray.h"
#include "Renderer/Shader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace HEngine {
Scene::Scene()
{
    // create default resources
    m_DefaultTexture = HEngine::Texture2D::Create("assets/textures/test.jpg");
    m_DefaultMaterial = MatLibrary.AddNewMaterial(std::string("DefaultMaterial"));
    m_DefaultMaterial->shader = CreateRef<HEngine::Shader>("assets/shaders/DefaultShader.glsl");

    std::vector<FVertex> verts = {
        { Vec3 { -0.5f, -0.5f, 0.0f }, Vec3 { 0.0f, 0.0f, 1.0f }, Vec2 { 0.0f, 0.0f }, Vec4 { 1.0f } },
        { Vec3 { 0.5f, -0.5f, 0.0f }, Vec3 { 0.0f, 0.0f, 1.0f }, Vec2 { 1.0f, 0.0f }, Vec4 { 1.0f } },
        { Vec3 { 0.5f, 0.5f, 0.0f }, Vec3 { 0.0f, 0.0f, 1.0f }, Vec2 { 1.0f, 1.0f }, Vec4 { 1.0f } },
        { Vec3 { -0.5f, 0.5f, 0.0f }, Vec3 { 0.0f, 0.0f, 1.0f }, Vec2 { 0.0f, 1.0f }, Vec4 { 1.0f } },
    };
    std::vector<uint32_t> indx = { 0, 1, 2, 2, 3, 0 };
    m_QuadVAO.reset(VertexArray::Create());
    Ref<VertexBuffer> vertexBuffer;
    vertexBuffer.reset(VertexBuffer::Create(verts, verts.size() * static_cast<uint32_t>(sizeof(Vec3) + sizeof(Vec3) + sizeof(Vec4))));
    BufferLayout layout = {
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float3, "a_Normal" },
        { ShaderDataType::Float4, "a_Color" }
    };
    vertexBuffer->SetLayout(layout);
    m_QuadVAO->AddVertexBuffer(vertexBuffer);

    Ref<IndexBuffer> indexBuffer;
    indexBuffer.reset(IndexBuffer::Create(indx.data(), static_cast<uint32_t>(indx.size())));
    m_QuadVAO->SetIndexBuffer(indexBuffer);

    m_DefaultShader = std::make_shared<Shader>("assets/shaders/DefaultShader.glsl");
}

Scene::~Scene()
{
}

void Scene::Initialize()
{
}

void Scene::setupScene()
{
}

void Scene::UpdateLightsInfo()
{
    PointLightList.clear();
    auto view = m_Registry.view<PointLightComponent, TransformComponent>();
    for (auto& entt : view) {
        auto [tc, light] = view.get<TransformComponent, PointLightComponent>(entt);
        if (light.bAffectWorld) {
            PointLightList.push_back({ tc.Position, light.m_Color * light.m_Intensity });
        }
    }
}

Entity Scene::CreateEntity(const std::string& name /*= std::string() */)
{
    Entity entt = { m_Registry.create(), this };
    entt.AddComponent<TransformComponent>();
    auto& tag = entt.AddComponent<TagComponent>();
    tag.tag = name.empty() ? "Unnamed Entity" : name;
    return entt;
}

void Scene::DestroyEntity(Entity entity)
{
    m_Registry.destroy(entity);
}

}
