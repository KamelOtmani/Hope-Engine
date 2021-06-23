#pragma once
#include "Renderer/Core/Renderer.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/GBuffer.h"

namespace HEngine
{


    class SceneRenderer
    {
    public:
        SceneRenderer();
        ~SceneRenderer() = default;

        void Initialize();
        void Update(EditorCamera& camera,float dt);
        void OnViewportResize(uint32_t Width, uint32_t Height);

        uint32_t getOutputTextureID() {return m_OutputFramebuffer->getColorAttachement(0);};

        void UploadCommunToShader(Shader* shader, EditorCamera* camera);
        void RenderGeometryPass(EditorCamera& camera);

        Ref<Scene> m_Scene;
        // Debug
        std::map<std::string, uint32_t> DebugFBTexturesID;
        
        //// SSAO
        //float radius = 0.5;
        //float bias = 0.025;
        //float scale = 16;
        //float power = 1;
    private:
        // TODO : implement this as a render pass with shaders 
        Ref<Framebuffer> m_OutputFramebuffer;
        Ref<Framebuffer> m_GeometryPass;
        Ref<Framebuffer> m_DefferedShading;
        //Ref<Framebuffer> m_SSAOPass;
        GBuffer gBuffer;

        // screen Quad
        Ref<VertexArray> m_ScreenQuadVAO;

        // Shaders
        Ref<Shader> m_CompositingShader;
        Ref<Shader> m_DefferedLightingShader;

        //Ref<Shader> m_SSAOShader;

        //// Parameters
        //Vector<Vec3> SSAOKernel;
        //Vector<Vec3> SSAONoise;
        //Ref<Texture2D> SSAONoiseTexture;
    };
};

