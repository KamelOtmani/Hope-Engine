#pragma once
#include "Renderer/RenderPass.h"
#include "Renderer/GBuffer.h"

namespace HEngine
{
    class Shader;

    class DefferedLightingPass : public RenderPass
    {
    public:
        DefferedLightingPass();


        void Initialize() override;
        void Render() override;
        Ref<Shader> m_MainShader;
    private:
        void UploadLightInfoToShader();
        void UploadGBufferTextureToShader();
        

    private:
        GBuffer gbuffer;
    };
}
