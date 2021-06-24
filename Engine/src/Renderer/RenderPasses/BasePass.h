#pragma once

#include "Renderer/RenderPass.h"
namespace HEngine
{
    class BasePass : public RenderPass
    {
    public:
        BasePass();
        ~BasePass();

        void Initialize() override;
        void Render() override {};

        void BeginFrame();
        void EndFrame();

    };
}