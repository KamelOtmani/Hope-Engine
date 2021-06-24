#include "hpch.h"
#include "BasePass.h"

#include "Renderer/SceneRenderer.h"

namespace HEngine
{
    BasePass::BasePass()
    {

    }

    BasePass::~BasePass()
    {

    }

    void BasePass::Initialize()
    {
        FramebufferSpecification BasePassFBSpec;
        BasePassFBSpec.width = 1600;
        BasePassFBSpec.height = 900;
        //                                  Postion                                  Normal+ specular power (roughness)     Albedo + Specular
        BasePassFBSpec.Attachements = { { FBTextureFormat::RGBA16F,Vec4(0.0f) }, { FBTextureFormat::RGBA16F,Vec4(0.0f) }, { FBTextureFormat::RGBA16F,Vec4(0.0f) }, FBTextureFormat::Depth };
        m_MainFramebuffer = Framebuffer::Create(BasePassFBSpec);
    }

    void BasePass::BeginFrame()
    {
        m_MainFramebuffer->Bind();
        //m_MainFramebuffer->ClearAttachment(0);
        //m_MainFramebuffer->ClearAttachment(1);
        //m_MainFramebuffer->ClearAttachment(2);
        RHICommand::SetClearColor(Vec4(0.0f));
        RHICommand::Clear();
    }

    void BasePass::EndFrame()
    {
        m_MainFramebuffer->UnBind();
    }

}