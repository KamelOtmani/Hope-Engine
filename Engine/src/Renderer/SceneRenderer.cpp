#include "hpch.h"
#include "SceneRenderer.h"

#include <glad/glad.h>
#include <random>

#include "ECS/Components.h"

namespace HEngine
{
    static float lerp(float a, float b, float f)
    {
        return a + f * (b - a);
    }

    SceneRenderer::SceneRenderer()
    {
        std::vector<FVertex> verts = {
    { Vec3{-1.0f, 1.0f, 0.0f }  ,Vec3{0.0f,0.0f,1.0f },Vec2{0.0f,1.0f},  Vec4{1.0f}},
    { Vec3{-1.0f, -1.0f, 0.0f}    ,Vec3{0.0f,0.0f,1.0f },Vec2{0.0f,0.0f},  Vec4{1.0f}},
    { Vec3{1.0f, 1.0f, 0.0f}     ,Vec3{0.0f,0.0f,1.0f },Vec2{1.0f,1.0f},  Vec4{1.0f}},
    { Vec3{1.0f, -1.0f, 0.0f}    ,Vec3{0.0f,0.0f,1.0f },Vec2{1.0f,0.0f},  Vec4{1.0f}},
        };
        std::vector<uint32_t> indx = { 0, 1,2,2,1,3 };
        m_ScreenQuadVAO.reset(VertexArray::Create());
        Ref<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::Create(verts, verts.size() * static_cast<uint32_t>(sizeof(Vec3) + sizeof(Vec3)+ sizeof(Vec2) + sizeof(Vec4))));
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Float4, "a_Color" }
        };
        vertexBuffer->SetLayout(layout);
        m_ScreenQuadVAO->AddVertexBuffer(vertexBuffer);

        Ref<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::Create(indx.data(), static_cast<uint32_t>(indx.size())));
        m_ScreenQuadVAO->SetIndexBuffer(indexBuffer);

    }

    void SceneRenderer::Initialize()
    {
       FramebufferSpecification BasePassFBSpec;
        BasePassFBSpec.width = 1600;
        BasePassFBSpec.height = 900;
        //                                  Postion                 Normal+ specular power (roughness)     Albedo + Specular
        BasePassFBSpec.Attachements = { FBTextureFormat::RGBA16F,FBTextureFormat::RGBA16F,FBTextureFormat::RGBA16F,FBTextureFormat::Depth };
        m_GeometryPass = Framebuffer::Create(BasePassFBSpec);
        gBuffer.Position = m_GeometryPass->getColorAttachement(0);
        gBuffer.Normal = m_GeometryPass->getColorAttachement(1);
        gBuffer.Albedo = m_GeometryPass->getColorAttachement(2);

        DebugFBTexturesID["GBuffer.Position"] = gBuffer.Position;
        DebugFBTexturesID["GBuffer.Normal"] = gBuffer.Normal ;
        DebugFBTexturesID["GBuffer.Albedo"] = gBuffer.Albedo ;

        // Deffered Shading Pass
        FramebufferSpecification defferedFBSpec;
        defferedFBSpec.width = 1600;
        defferedFBSpec.height = 900;
        defferedFBSpec.Attachements = { FBTextureFormat::RGBA16F };
        m_DefferedShading = Framebuffer::Create(defferedFBSpec);
        DebugFBTexturesID["Deffered Shading Result"] = m_DefferedShading->getColorAttachement(0);





        // Deffered Shading Pass
        FramebufferSpecification finalFBSpec;
        finalFBSpec.width = 1600;
        finalFBSpec.height = 900;
        finalFBSpec.Attachements = { FBTextureFormat::RGBA8 };
        m_OutputFramebuffer = Framebuffer::Create(finalFBSpec);
        DebugFBTexturesID["Final Color"] = m_OutputFramebuffer->getColorAttachement(0);


        m_CompositingShader = CreateRef<Shader>("assets/shaders/Compositing.glsl");
        m_DefferedLightingShader = CreateRef<Shader>("assets/shaders/DefferedShading.glsl");
        //m_SSAOShader = CreateRef<Shader>("assets/shaders/SSAO.glsl");
        Renderer::Initialise();
    }

    void SceneRenderer::Update(EditorCamera& camera,float dt)
    {
        if (m_Scene)
        {
            // BasePass
            m_GeometryPass->Bind();
            RHICommand::SetClearColor(Vec4(0.1f));
            RHICommand::Clear();
            RenderGeometryPass(camera);
            m_GeometryPass->UnBind();

            // Deffered Shading
            m_DefferedShading->Bind();
            RHICommand::SetClearColor(Vec4(0.f));
            RHICommand::Clear();
            m_DefferedLightingShader->Bind();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, gBuffer.Position);
            m_DefferedLightingShader->SetInt("gBuffer.Position", 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, gBuffer.Normal);
            m_DefferedLightingShader->SetInt("gBuffer.Normal", 1);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, gBuffer.Albedo);
            m_DefferedLightingShader->SetInt("gBuffer.Albedo", 2);
            m_Scene->UpdateLightsInfo();
            UploadCommunToShader(m_DefferedLightingShader.get(), &camera);
            m_DefferedLightingShader->SetInt("u_num_point_light", static_cast<int>(m_Scene->PointLightList.size()));
            int i = 0;
            for (auto& pointlight : m_Scene->PointLightList)
            {
                m_DefferedLightingShader->SetFloat3("u_PointLights[" + std::to_string(i) + "].position", pointlight.Position);
                m_DefferedLightingShader->SetFloat4("u_PointLights[" + std::to_string(i) + "].color", pointlight.Color);
                i++;
            }
            m_DefferedLightingShader->SetFloat3("u_CameraPositionWS", camera.GetPosition());
            Renderer::Submit(m_ScreenQuadVAO);
            m_DefferedShading->UnBind();



            // render Compositing pass
            m_OutputFramebuffer->Bind();
            //RHICommand::SetClearColor(Vec4(0.f));
            //RHICommand::Clear();
            m_CompositingShader->Bind();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_DefferedShading->getColorAttachement(0));
            Renderer::Submit(m_ScreenQuadVAO);
            m_OutputFramebuffer->UnBind();
        }
    }

    void SceneRenderer::OnViewportResize(uint32_t Width, uint32_t Height)
    {
        m_OutputFramebuffer->Resize(Width, Height);
    }

    void SceneRenderer::UploadCommunToShader(Shader* shader,EditorCamera* camera)
    {
        shader->SetMat4("u_View2World", glm::inverse(camera->GetViewMatrix()));
        shader->SetMat4("u_World2View", camera->GetViewMatrix());
        shader->SetMat4("u_View2Clip", camera->GetProjectionMatrix());
        shader->SetMat4("u_World2Clip", camera->GetViewProjection());
    }

    void SceneRenderer::RenderGeometryPass(EditorCamera& camera)
    {


        {
            auto group = m_Scene->m_Registry.view<TransformComponent, MeshRendererComponent>();
            for (auto& entity : group)
            {
                auto [xform, mesh] = group.get<TransformComponent, MeshRendererComponent>(entity);
                if (!mesh.bEmpty && mesh.bSouldRender)
                {
                    if (mesh.material->shader != nullptr)
                    {

                        mesh.material->shader->Bind();
                        mesh.material->shader->SetFloat3("u_CameraPositionWS", camera.GetPosition());
                        mesh.material->shader->SetMat4("u_Transform", xform.Matrix());
                        UploadCommunToShader(mesh.material->shader.get(),&camera);
                        mesh.material->ApplyMaterial();
                    }
                    mesh.vertexArray->Bind();
                    RHICommand::DrawIndexed(mesh.vertexArray);
                }

            }
        }

        auto group2 = m_Scene->m_Registry.view<TransformComponent, SpriteRendererComponent>();
        for (auto entity : group2)
        {
            auto [xform, sprite] = group2.get<TransformComponent, SpriteRendererComponent>(entity);
            if (sprite.m_Shader)
            {
                sprite.m_Shader->Bind();
                sprite.m_Shader->SetFloat4("u_Color", sprite.m_Color);
                sprite.m_Shader->SetMat4("u_Transform", xform.Matrix());
                UploadCommunToShader(sprite.m_Shader, &camera);
            }
            m_Scene->m_QuadVAO->Bind();
            RHICommand::DrawIndexed(m_Scene->m_QuadVAO);

        }
    }

}


//// Initialisation 
//FramebufferSpecification bloomfb;
//bloomfb.width = 1600;
//bloomfb.height = 900;
//bloomfb.Attachements = { FBTextureFormat::RGBA8 };
//
//for (int i = 1; i <= BloomIterations; i++)
//{
//    bloomfb.width /= BloomDownsampleFactor;
//    bloomfb.height /= BloomDownsampleFactor;
//    m_BloomPass.emplace_back(Framebuffer::Create(bloomfb));
//    DebugFBTexturesID["Bloom Downsample " + std::to_string(bloomfb.width) +
//        "x" + std::to_string(bloomfb.height)] = m_BloomPass.back()->getColorAttachement(0);
//}
//int i = 0;
//for (auto& blur : m_BloomPass)
//{
//    auto baseWidth = blur->getSpecification().width;
//    auto baseHeight = blur->getSpecification().height;
//    auto steps = (m_BloomPass[0]->getSpecification().width / baseWidth) / BloomDownsampleFactor + 1;
//    HLOG(" i = {0} , steps = {1}", i, steps);
//    for (int j = 1; j <= steps; j++)
//    {
//        bloomfb.width = baseWidth * BloomDownsampleFactor * j;
//        bloomfb.height = baseHeight * BloomDownsampleFactor * j;
//        m_BloomUpscale.emplace_back(Framebuffer::Create(bloomfb));
//        DebugFBTexturesID["Bloom Upsample  " + std::to_string(bloomfb.width) +
//            "x" + std::to_string(bloomfb.height) + " from " + std::to_string(bloomfb.width * j / BloomDownsampleFactor) +
//            "x" + std::to_string(bloomfb.height * j / BloomDownsampleFactor)] = m_BloomUpscale.back()->getColorAttachement(0);
//    }
//    i++;
//}

//// Bloom Downscaling + blurpass
//for (auto& fb : m_BloomPass)
//{
//    fb->Bind();
//    // Horizontal Blur
//    m_HorizontalBlurShader->Bind();
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, m_GeometryPass->getColorAttachement(1));
//    m_HorizontalBlurShader->SetInt("u_MainTexture", 0);
//    m_HorizontalBlurShader->SetFloat("u_TextureResolutionX", fb->getSpecification().width);
//    m_HorizontalBlurShader->SetFloat("u_TextureResolutionY", fb->getSpecification().height);
//    Renderer::Submit(m_ScreenQuadVAO);
//
//    // Vertical Blur
//    m_VerticalBlurShader->Bind();
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, fb->getColorAttachement(0));
//    m_VerticalBlurShader->SetInt("u_MainTexture", 0);
//    m_VerticalBlurShader->SetFloat("u_TextureResolutionX", fb->getSpecification().width);
//    m_VerticalBlurShader->SetFloat("u_TextureResolutionY", fb->getSpecification().height);
//    Renderer::Submit(m_ScreenQuadVAO);
//    fb->UnBind();
//}
//
//// BasePass
//m_GeometryPass->Bind();
//Renderer::PrepareScene(m_Scene.get());
//Renderer::SubmitScene(m_Scene.get(), camera);
//m_GeometryPass->UnBind();
//Vector<uint32_t> finalbloomTextures;
//int i = 0;
//// Bloom Upscaling + blurpass
//for (auto& blur : m_BloomPass)
//{
//    auto baseWidth = blur->getSpecification().width;
//    auto baseHeight = blur->getSpecification().height;
//    auto steps = (m_BloomPass[0]->getSpecification().width / baseWidth) / BloomDownsampleFactor + 1;
//    HLOG(" i = {0} , steps = {1}", i, steps);
//    auto id = blur->getColorAttachement(0);
//    for (int j = 0; j < steps; j++)
//    {
//
//        m_UpscaleShader->Bind();
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, id);
//        m_UpscaleShader->SetInt("u_MainTexture", 0);
//        Renderer::Submit(m_ScreenQuadVAO);
//
//        m_BloomUpscale[i + j]->Bind();
//        // Horizontal Blur
//        m_HorizontalBlurShader->Bind();
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, id);
//        m_HorizontalBlurShader->SetInt("u_MainTexture", 0);
//        m_HorizontalBlurShader->SetFloat("u_TextureResolutionX", m_BloomUpscale[i + j]->getSpecification().width);
//        m_HorizontalBlurShader->SetFloat("u_TextureResolutionY", m_BloomUpscale[i + j]->getSpecification().height);
//        Renderer::Submit(m_ScreenQuadVAO);
//
//        // Vertical Blur
//        m_VerticalBlurShader->Bind();
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, m_BloomUpscale[i + j]->getColorAttachement(0));
//        m_VerticalBlurShader->SetInt("u_MainTexture", 0);
//        m_VerticalBlurShader->SetFloat("u_TextureResolutionX", m_BloomUpscale[i + j]->getSpecification().width);
//        m_VerticalBlurShader->SetFloat("u_TextureResolutionY", m_BloomUpscale[i + j]->getSpecification().height);
//        Renderer::Submit(m_ScreenQuadVAO);
//        id = m_BloomUpscale[i + j]->getColorAttachement(0);
//        m_BloomUpscale[i + j]->UnBind();
//        if ((steps - j) <= 1)
//        {
//            finalbloomTextures.push_back(id);
//        }
//    }
//    i++;
//}
//
//// BasePass
//m_GeometryPass->Bind();
//Renderer::PrepareScene(m_Scene.get());
//Renderer::SubmitScene(m_Scene.get(), camera);
//m_GeometryPass->UnBind();
//
//m_OutputFramebuffer->Bind();
//// Setup composition pass
//m_CompositingShader->Bind();
//glActiveTexture(GL_TEXTURE0);
//glBindTexture(GL_TEXTURE_2D, m_GeometryPass->getColorAttachement(0));
//m_CompositingShader->SetInt("u_MainTexture", 0);
//m_CompositingShader->SetFloat("u_Exposure", 0);
//for (auto& tex : finalbloomTextures)
//{
//    glActiveTexture(GL_TEXTURE0 + i);
//    glBindTexture(GL_TEXTURE_2D, tex);
//    m_CompositingShader->SetInt("u_BloomTextures[" + std::to_string(i) + "]", i);
//}


//// SSAO 
//            // SSAO
//m_SSAOPass->Bind();
//RHICommand::SetClearColor(Vec4(0.f));
//RHICommand::Clear();
//m_SSAOShader->Bind();
//glActiveTexture(GL_TEXTURE0);
//glBindTexture(GL_TEXTURE_2D, gBuffer.Position);
//m_SSAOShader->SetInt("gBuffer.Position", 0);
//glActiveTexture(GL_TEXTURE1);
//glBindTexture(GL_TEXTURE_2D, gBuffer.Normal);
//m_SSAOShader->SetInt("gBuffer.Normal", 1);
//glActiveTexture(GL_TEXTURE2);
//glBindTexture(GL_TEXTURE_2D, gBuffer.Albedo);
//m_SSAOShader->SetInt("gBuffer.Albedo", 2);
//glActiveTexture(GL_TEXTURE3);
//glBindTexture(GL_TEXTURE_2D, SSAONoiseTexture->getID());
//m_SSAOShader->SetInt("u_NoiseTexture", 3);
//
//m_SSAOShader->SetInt("u_ResolutionX", 1600);
//m_SSAOShader->SetInt("u_ResolutionY", 900);
//m_SSAOShader->SetFloat("radius", radius);
//m_SSAOShader->SetFloat("bias", bias);
//m_SSAOShader->SetFloat("scale", scale);
//m_SSAOShader->SetFloat("power", power);
//UploadCommunToShader(m_SSAOShader.get(), &camera);
//i = 0;
//for (auto& sample : SSAOKernel)
//{
//    m_SSAOShader->SetFloat3("samples[" + std::to_string(i) + "]", sample);
//    i++;
//}
//Renderer::Submit(m_ScreenQuadVAO);
//m_SSAOPass->UnBind();

        //// SSAO
        //FramebufferSpecification SsaoFBSpec;
        //SsaoFBSpec.width = 1600;
        //SsaoFBSpec.height = 900;
        //SsaoFBSpec.Attachements = { FBTextureFormat::RGBA16F };
        //m_SSAOPass = Framebuffer::Create(SsaoFBSpec);
        //DebugFBTexturesID["SSAO"] = m_SSAOPass->getColorAttachement(0);
        ////SSAO Samples 
        //std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between [0.0, 1.0]
        //std::default_random_engine generator;
        //for (unsigned int i = 0; i < 64; ++i)
        //{
        //    Vec3 sample(
        //        randomFloats(generator) * 2.0 - 1.0,
        //        randomFloats(generator) * 2.0 - 1.0,
        //        randomFloats(generator)
        //    );
        //    sample = glm::normalize(sample);
        //    sample *= randomFloats(generator);
        //    float scale = (float)i / 64.0;
        //    scale = lerp(0.1f, 1.0f, scale * scale);
        //    sample *= scale;
        //    SSAOKernel.push_back(sample);
        //}
        //SSAONoiseTexture = Texture2D::Create("assets/textures/BlueNoise1_16.png");
        //DebugFBTexturesID["SSAO Noise"] = SSAONoiseTexture->getID();