#pragma once

#include "entt.hpp"

namespace HEngine {
    class Entity;
    class VertexArray;
    class Shader;
    class Serializer;

    class Scene 
    {
    public:
        Scene();
        ~Scene();

        void Initialize();

        void setupScene();

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        std::string m_SceneName = "Untitled Scene";
        entt::registry m_Registry;
        Ref<VertexArray> m_QuadVAO;
        Ref<Shader> m_DefaultShader;
    private:

        friend class Entity;
        friend class Renderer;
        friend class SceneSerializer;
    };

}
