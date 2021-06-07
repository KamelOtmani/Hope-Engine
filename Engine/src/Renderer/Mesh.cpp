#include "hpch.h"
#include "Mesh.h"

void HEngine::Mesh::setInternal(const std::vector<FVertex>& data, const std::vector<uint32_t>& indices)
{
    _data = data;
    _indices = indices;
}

std::vector<FVertex> HEngine::Mesh::getData()
{
    return _data;
}

std::vector<uint32_t> HEngine::Mesh::getIndicies()
{
    return _indices;
}
