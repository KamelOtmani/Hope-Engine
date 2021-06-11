#include "hpch.h"
#include "Renderer\Material.h"

namespace HEngine
{

    Ref<HEngine::Material> MaterialLibrary::AddNewMaterial(std::string& name)
    {
        return Library.emplace_back(CreateRef<Material>(name));
    }

    Ref<HEngine::Material> MaterialLibrary::GetMaterialbyName(std::string& name)
    {
        for (auto& mat : Library)
        {
            if (name == mat->name)
            {
                return mat;
            }
        }
        return nullptr;
    }

    Material::Material(std::string& Name)
        : name(Name)  {}

    // THIS FUNCTION DO NOT BINF THE SHADER
    // call after binding the resourse
    void Material::ApplyMaterial()
    {
        shader->SetFloat4("u_Color",Color);
    }

}

