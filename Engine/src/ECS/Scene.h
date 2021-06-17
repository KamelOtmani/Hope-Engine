#pragma once

#include "entt.hpp"
#include "Renderer/Material.h"
#include "Renderer/Texture.h"

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
        void UpdateLightsInfo();

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        MaterialLibrary MatLibrary;
        std::string m_SceneName = "Untitled Scene";
        entt::registry m_Registry;
        Ref<VertexArray> m_QuadVAO;
        Ref<Shader> m_DefaultShader;
        Ref<HEngine::Texture2D> m_DefaultTexture;
        Ref<HEngine::Material> m_DefaultMaterial;
        
        struct PointLight 
        {
            Vec3 Position;
            Vec4 Color;
        };

        Vector<PointLight> PointLightList;
    private:

        friend class Entity;
        friend class Renderer;
        friend class SceneSerializer;
    };

}
