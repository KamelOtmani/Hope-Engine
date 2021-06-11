#pragma once
#include "Core/Core.h"
#include "Renderer/Commun.h"

#include <string>


namespace HEngine
{
    struct Model
    {
        Vector<FVertex> verts;
        Vector<uint32_t> indices;
    };
    class AssetImporter
    {
    public:
        static Model ImportModel(std::string& path);

    };
}