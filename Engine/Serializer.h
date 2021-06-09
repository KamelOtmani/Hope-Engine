#pragma once

#include "ECS/Scene.h"

namespace HEngine
{
    class SceneSerializer
    {
    public:
        SceneSerializer(const Ref<Scene>& scene);

        void Serialize(const std::string& path);
        bool Deserialize(const std::string& path);
    private:
        Ref<Scene> m_Scene;
    };
}