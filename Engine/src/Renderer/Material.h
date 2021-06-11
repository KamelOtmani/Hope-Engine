#pragma once
#include "Core/Core.h"
#include "HTypes.h"
#include "Renderer/Shader.h"
#include <string>

class Shader;

namespace HEngine {

    class Material
    {
    public:
        Material() = default;
        Material(std::string& Name);
        ~Material()= default;

        void ApplyMaterial();

        Vec4 Color{ 1.0f };
        Ref<HEngine::Shader> shader;
        std::string name = "unnamed material";
    };

    class MaterialLibrary
    {
    public:
        MaterialLibrary() = default;

        Ref<Material> AddNewMaterial(std::string& name);
        Ref<Material> GetMaterialbyName(std::string& name);
        Vector<Ref<Material>> Library;
    };
}

