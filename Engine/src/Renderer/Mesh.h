#pragma once

#include "Renderer\Commun.h"

namespace HEngine {


    class Mesh
    {
        Mesh() = default;
        ~Mesh() = default;

        void setInternal(const std::vector<FVertex>& data, const std::vector<uint32_t>& indices);
        std::vector<FVertex> getData();
        std::vector<uint32_t> getIndicies();

    private:
        std::vector<FVertex> _data;
        std::vector<uint32_t> _indices;
    };

}
