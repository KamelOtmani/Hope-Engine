#include "hpch.h"
#include "Mesh.h"

namespace HEngine
{
    Mesh::Mesh(Vector<FVertex> vertices, Vector<unsigned int> indices)
        :m_Vertices(vertices), m_Indices(indices)
    {

    }
}

