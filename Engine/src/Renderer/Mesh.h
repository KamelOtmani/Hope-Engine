#pragma once

#include "Core/Core.h"
#include "Renderer\Commun.h"
#include "Renderer\Commun.h"

namespace HEngine {

    class Mesh
    {
    public:
        Mesh(Vector<FVertex> vertices, Vector<unsigned int> indices);

        // mesh data
        Vector<FVertex> m_Vertices;
        Vector<uint32_t> m_Indices;
        //Vector<Texture> textures;
    };

}
