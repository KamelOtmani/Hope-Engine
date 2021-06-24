#pragma once
#include "Core/Core.h"
#include "HTypes.h"
#include "Renderer/Shader.h"
#include <string>


namespace HEngine {

    class Shader;
    class Texture2D;

    class Material
    {
    public:
        Material() = default;
        Material(std::string& Name);
        ~Material()= default;

        void ApplyMaterial();

        Vec4 Color{ 1.0f };
        float Roughness{ 0.3f };
        float Metalic{ 0.0f };
        //float Specular{ 0.3f };

        Ref<HEngine::Texture2D> m_AlbedoTexture;
        Ref<HEngine::Texture2D> m_RoughnessTexture;
        Ref<HEngine::Texture2D> m_MetalicTexture;

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

