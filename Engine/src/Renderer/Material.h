#pragma once

#include <memory>

class Shader;

namespace HEngine {

    class Material
    {
        Material();
        ~Material();

        std::shared_ptr<Shader> shader;
    };
}

