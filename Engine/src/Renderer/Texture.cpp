#include "hpch.h"
#include "Texture.h"


#include "Core/Renderer.h"

#include "Platform/OpenGl/Renderer/GLTexture.h"

namespace HEngine
{
    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch (Renderer::GetAPI())
        {
        case RHI::API::None:    HENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RHI::API::OpenGL:  return CreateRef<GLTexture2D>(width, height);
        }

        HENGINE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const std::string& path)
    {
        switch (Renderer::GetAPI())
        {
        case RHI::API::None:    HENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RHI::API::OpenGL:  return CreateRef<GLTexture2D>(path);
        }

        HENGINE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}