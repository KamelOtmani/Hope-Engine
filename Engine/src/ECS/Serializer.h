#pragma once

#include "ECS/Scene.h"

namespace HEngine {

class Serializer {
public:
    Serializer(const Ref<Scene>& scene);

    void Serialize(const std::string& filepath);

    bool Deserialize(const std::string& filepath);

private:
    Ref<Scene> m_Scene;
};

}
