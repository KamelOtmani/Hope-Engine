#pragma once
#include "Renderer/RHI/RHI.h"

namespace HEngine {
class OpenGLRenderer : public RHI {
public:
    void SetClearColor(const glm::vec4& color) override;
    void Clear() override;

    void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
};
}
