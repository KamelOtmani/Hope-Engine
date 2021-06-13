#pragma once
#include "Core/Core.h"
#include "Renderer/Commun.h"

#include <string>


namespace HEngine
{
    class Scene;

    struct Model
    {
        Vector<FVertex> verts;
        Vector<uint32_t> indices;
    };
    class AssetImporter
    {
    public:
        static Model ImportModel(std::string& path);
        static void ImportScene(std::string& path, Scene* m_scene, float scale = 1.0f);

    };
}