#include "hpch.h"
#include "Texture.h"


#include "Core/Renderer.h"

#include "Platform/OpenGl/Renderer/GLTexture.h"

namespace HEngine
{
    Ref<Texture2D> Texture2D::Create(TextureSpecefication spec)
    {
        switch (Renderer::GetAPI())
        {
        case RHI::API::None:    HENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RHI::API::OpenGL:  return CreateRef<GLTexture2D>(spec);
        }

        HENGINE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path, bool bHDR)
    {
        switch (Renderer::GetAPI())
        {
        case RHI::API::None:    HENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RHI::API::OpenGL:  return CreateRef<GLTexture2D>(path, bHDR);
        }

        HENGINE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}