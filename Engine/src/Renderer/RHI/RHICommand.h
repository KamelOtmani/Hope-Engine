#pragma once

#include "Renderer/RHI/RHI.h"

namespace HEngine {

class RHICommand {
public:
    inline static void SetClearColor(const Vec4 color)
    {
        s_RHI->SetClearColor(color);
    }

    inline static void Clear()
    {
        s_RHI->Clear();
    }

    inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        s_RHI->DrawIndexed(vertexArray);
    }

private:
    static RHI* s_RHI;
};

}
