#include "hpch.h"
#include "Renderer\Material.h"
#include "Renderer\Texture.h"
#include <glad\glad.h>

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
    // call after binding the ressources
    void Material::ApplyMaterial()
    {
        shader->SetFloat4("u_Material.Color", Color);
        shader->SetFloat("u_Material.SpecularPower", SpecularPower);
        if (m_AlbedoTexture)
        {
            m_AlbedoTexture->Bind(0);
            shader->SetInt("u_Material.AlbedoTexture", 0);
            shader->SetBool("u_Material.hasAlbedoTexture", true);
        }
        else
        {
            shader->SetBool("u_Material.hasAlbedoTexture", false);
        }

        if (m_SpecularTexture)
        {
            m_SpecularTexture->Bind(1);
            shader->SetInt("u_Material.SpecularTexture", 1);
            shader->SetBool("u_Material.hasSpecularTexture", true);
        }
        else
        {
            shader->SetBool("u_Material.hasSpecularTexture", false);
        }
    }

}

