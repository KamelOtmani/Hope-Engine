#include "hpch.h"
#include "Framebuffer.h"

#include "Core/Core.h"

#include "Core/Renderer.h"
#include "Platform/OpenGl/Renderer/GLFramebuffer.h"


namespace HEngine
{

    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        switch (Renderer::GetAPI())
        {
        case RHI::API::None:    HENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RHI::API::OpenGL:  return CreateRef<GLFramebuffer>(spec);
        }
        HENGINE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}