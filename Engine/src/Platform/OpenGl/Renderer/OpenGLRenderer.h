#pragma once
#include "Renderer/RHI/RHI.h"

namespace HEngine {
    class OpenGLRenderer : public RHI
    {
    public:
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
    };
}

