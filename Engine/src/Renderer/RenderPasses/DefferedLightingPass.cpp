#include "hpch.h"
#include "DefferedLightingPass.h"
#include "Renderer/SceneRenderer.h"
#include <glad\glad.h>


namespace HEngine
{
    DefferedLightingPass::DefferedLightingPass()
    {
        m_MainShader = CreateRef<Shader>("assets/shaders/DefferedShading.glsl");
    }

    void DefferedLightingPass::Initialize()
    {
        FramebufferSpecification defferedFBSpec;
        defferedFBSpec.width = 1600;
        defferedFBSpec.height = 900;
        defferedFBSpec.Attachements = { FBTextureFormat::RGBA16F };
        m_MainFramebuffer = Framebuffer::Create(defferedFBSpec);
        gbuffer = m_SceneRenderer->gBuffer;
    }

    void DefferedLightingPass::Render()
    {
        m_MainFramebuffer->Bind();

        // Update shader uniforms
        m_MainShader->Bind();
        UploadGBufferTextureToShader();
        m_SceneRenderer->m_Scene->UpdateLightsInfo();
        UploadLightInfoToShader();

        // Render the pass

        Renderer::Submit(m_SceneRenderer->m_ScreenQuadVAO);
        m_MainFramebuffer->UnBind();
    }

    void DefferedLightingPass::UploadLightInfoToShader()
    {
        m_MainShader->SetInt("u_num_point_light", static_cast<int>(m_SceneRenderer->m_Scene->PointLightList.size()));
        int i = 0;
        for (auto& pointlight : m_SceneRenderer->m_Scene->PointLightList)
        {
            m_MainShader->SetFloat3("u_PointLights[" + std::to_string(i) + "].position", pointlight.Position);
            m_MainShader->SetFloat4("u_PointLights[" + std::to_string(i) + "].color", pointlight.Color);
            i++;
        }
        m_MainShader->SetFloat("u_SkyLightIntensity", static_cast<float>(m_SceneRenderer->SkyLightIntensity));
    }

    void DefferedLightingPass::UploadGBufferTextureToShader()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gbuffer.Position);
        m_MainShader->SetInt("gBuffer.Position", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gbuffer.Normal);
        m_MainShader->SetInt("gBuffer.Normal", 1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gbuffer.Albedo);
        m_MainShader->SetInt("gBuffer.Albedo", 2);
    }

}