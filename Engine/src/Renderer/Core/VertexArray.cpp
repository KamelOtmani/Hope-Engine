#include "hpch.h"
#include "VertexArray.h"

#include "Platform/OpenGL/Renderer/GLVertexArray.h"
#include "Renderer.h"

namespace HEngine {

VertexArray* VertexArray::Create()
{
    switch (Renderer::GetAPI()) {
    case RHI::API::None:
        HENGINE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RHI::API::OpenGL:
        return new OpenGLVertexArray();
    }

    HENGINE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}
