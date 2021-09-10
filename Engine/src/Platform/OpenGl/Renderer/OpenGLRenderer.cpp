#include "hpch.h"
#include "OpenGLRenderer.h"

#include <glad/glad.h>

namespace HEngine {

void OpenGLRenderer::SetClearColor(const Vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRenderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::DrawIndexed(const Ref<VertexArray>& vertexArray)
{
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}
}
