#pragma once

#include "entt.hpp"

namespace HEngine {
    class Entity;


    class Scene 
    {
    public:
        Scene();
        ~Scene();

        void setupScene();

        Entity CreateEntity(const std::string& name = std::string() );
    
        entt::registry m_Registry;
    private:

        friend class Entity;
        friend class Renderer;
    };

}
