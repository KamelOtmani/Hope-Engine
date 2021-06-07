#include "hpch.h"
#include "Scene.h"
#include "ECS\Entity.h"
#include "ECS\Components.h"

#include <glm/gtc/matrix_transform.hpp>

namespace HEngine
{
    Scene::Scene()
    {
        entt::entity entt = m_Registry.create();
    }

    Scene::~Scene()
    {
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
