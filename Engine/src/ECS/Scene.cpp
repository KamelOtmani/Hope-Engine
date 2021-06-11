#include "hpch.h"
#include "Scene.h"
#include "ECS\Entity.h"
#include "ECS\Components.h"

#include "Renderer/Core/VertexArray.h"
#include "Renderer/Shader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace HEngine
{
    Scene::Scene()
    {
        std::vector<FVertex> verts = {
            { Vec3{-0.5f, -0.5f, 0.0f }, Vec4{1.0f}},
            { Vec3{0.5f, -0.5f, 0.0f},   Vec4{1.0f}},
            { Vec3{0.5f, 0.5f, 0.0f} ,   Vec4{1.0f}},
            { Vec3{-0.5f, 0.5f, 0.0f},   Vec4{1.0f}},
        };
        std::vector<uint32_t> indx = { 0, 1, 2, 2, 3, 0 };
        m_QuadVAO.reset(VertexArray::Create());
        Ref<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::Create(verts, verts.size() * (sizeof(Vec3) + sizeof(Vec4))));
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" }
        };
        vertexBuffer->SetLayout(layout);
        m_QuadVAO->AddVertexBuffer(vertexBuffer);

        Ref<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::Create(indx.data(), indx.size()));
        m_QuadVAO->SetIndexBuffer(indexBuffer);


        m_DefaultShader = std::make_shared<Shader>("assets/shaders/DefaultShader.glsl");
    }

    Scene::~Scene()
    {
    }

    void Scene::Initialize()
    {
        auto view = m_Registry.view<MeshRendererComponent>();
        for (auto entity : view)
        {
            auto& mesh = view.get<MeshRendererComponent>(entity);
            // setup meshes
        }
    }

    void Scene::setupScene()
    {
    }

    Entity Scene::CreateEntity(const std::string& name /*= std::string() */)
    {
        Entity entt = { m_Registry.create(),this };
        entt.AddComponent<TransformComponent>();
        auto& tag = entt.AddComponent<TagComponent>();
        tag.tag = name.empty() ? "Unnamed Entity" : name;
        return entt;
    }

    void Scene::DestroyEntity(Entity entity )
    {
        m_Registry.destroy(entity);
    }

}
