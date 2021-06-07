#include "hpch.h"

#include "GLFramebuffer.h"

namespace HEngine
{
    GLFramebuffer::GLFramebuffer(const FramebufferSpecification& spec)
        : m_Specification(spec)
    {
        Invalidate();
    }

    GLFramebuffer::~GLFramebuffer()
    {
        glDeleteFramebuffers(1, &m_ID);
        glDeleteTextures(1, &m_ColorAttachementID);
        glDeleteTextures(1, &m_DepthAttachementID);
    }

    void GLFramebuffer::Invalidate()
    {
        // check if the ID is valid = Framebuffer is set
        if (m_ID)
        {
            // this mean we need to delete it and recrteate one new with the new spec
            glDeleteFramebuffers(1, &m_ID);
            glDeleteTextures(1, &m_ColorAttachementID);
            glDeleteTextures(1, &m_DepthAttachementID);
        }
        // we create the frame buffer object and bind it 
        glCreateFramebuffers(1, &m_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

        // then we create the attachement textures 
        // TODO : make them dynamic (an array of attachement ?)
        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachementID);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachementID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Specification.width, m_Specification.height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // bind the attachement texture to the frame buffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachementID, 0);

        // create the depth texture 
        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachementID);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachementID);
        //glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.width, m_Specification.height);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.width, m_Specification.height, 0,
            GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

        // and bind it to the framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachementID, 0);

        // check if the fb is complete
        HASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete, see the consol for more details !");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
        glViewport(0, 1, m_Specification.width, m_Specification.height);
    }

    void GLFramebuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    void GLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        m_Specification.width = width;
        m_Specification.height = height;
        Invalidate();
    }

}