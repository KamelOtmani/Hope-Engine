#pragma once
#include "Renderer/Framebuffer.h"

namespace HEngine
{
    class SceneRenderer;


    class RenderPass
    {
    public:
        virtual ~RenderPass() = default;

        virtual void Initialize() = 0;
        virtual void Render() = 0;

        Ref<Framebuffer>& getFrameBuffer() { return m_MainFramebuffer; }
        SceneRenderer* getSceneRenderer() { return m_SceneRenderer; }
        void setSceneRenderer(SceneRenderer* scn) { m_SceneRenderer = scn; }
    protected:
        Ref<Framebuffer> m_MainFramebuffer;
        SceneRenderer* m_SceneRenderer;
    };

}

