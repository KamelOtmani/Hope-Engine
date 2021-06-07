#pragma once

#include <memory>

class Shader;

namespace HEngine {

    class Material
    {
        Material();
        ~Material();

        Ref<Shader> shader;
    };
}

