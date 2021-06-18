#include "hpch.h"

#include "GLFramebuffer.h"

namespace HEngine
{
    namespace Utils {
        static GLenum TextureTarget(bool multisampled)
        {
            return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
        {
            glCreateTextures(TextureTarget(multisampled), count, outID);
        }

        static void BindTexture(bool multisampled, uint32_t id)
        {
            glBindTexture(TextureTarget(multisampled), id);
        }

        static void AttachColorTexture(uint32_t id, int samples, GLenum format, uint32_t width, uint32_t height, int index)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
        }

        static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
        {
            bool multisampled = samples > 1;
            if (multisampled)
            {
                glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
            }
            else
            {
                glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
        }

        static bool IsDepthFormat(FBTextureFormat format)
        {
            switch (format)
            {
            case FBTextureFormat::DEPTH24STENCIL8:  return true;
            }

            return false;
        }
    }

    GLFramebuffer::GLFramebuffer(const FramebufferSpecification& spec)
        : m_Specification(spec)
    {
        for (auto& spec : m_Specification.Attachements.Textures)
        {
            if (!Utils::IsDepthFormat(spec.textureFormat))
                m_ColorAttachmentSpecifications.emplace_back(spec);
            else
                m_DepthAttachmentSpecification = spec;
        }
        Invalidate();
    }

    GLFramebuffer::~GLFramebuffer()
    {
        glDeleteFramebuffers(1, &m_ID);
        glDeleteTextures(m_ColorAttachementsID.size(), m_ColorAttachementsID.data());
        glDeleteTextures(1, &m_DepthAttachementID);
    }

    void GLFramebuffer::Invalidate()
    {
        // check if the ID is valid = Framebuffer is set
        if (m_ID)
        {
            // this mean we need to delete it and recreate one new with the new spec
            glDeleteFramebuffers(1, &m_ID);
            glDeleteTextures(m_ColorAttachementsID.size(), m_ColorAttachementsID.data());
            glDeleteTextures(1, &m_DepthAttachementID);

            m_ColorAttachementsID.clear();
            m_DepthAttachementID = 0;
        }
        // we create the frame buffer object and bind it 
        glCreateFramebuffers(1, &m_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

        // then we create the attachment textures 

        bool multisample = m_Specification.Samples > 1;

        // Attachments
        if (m_ColorAttachmentSpecifications.size())
        {
            m_ColorAttachementsID.resize(m_ColorAttachmentSpecifications.size());
            Utils::CreateTextures(multisample, m_ColorAttachementsID.data(), m_ColorAttachementsID.size());

            for (size_t i = 0; i < m_ColorAttachementsID.size(); i++)
            {
                Utils::BindTexture(multisample, m_ColorAttachementsID[i]);
                switch (m_ColorAttachmentSpecifications[i].textureFormat)
                {
                case FBTextureFormat::RGBA8:
                    Utils::AttachColorTexture(m_ColorAttachementsID[i], m_Specification.Samples, GL_RGBA8,
                        m_Specification.width, m_Specification.height, i);
                    break;
                }
            }
        }

        if (m_DepthAttachmentSpecification.textureFormat != FBTextureFormat::None)
        {
            Utils::CreateTextures(multisample, &m_DepthAttachementID, 1);
            Utils::BindTexture(multisample, m_DepthAttachementID);
            switch (m_DepthAttachmentSpecification.textureFormat)
            {
            case FBTextureFormat::DEPTH24STENCIL8:
                Utils::AttachDepthTexture(m_DepthAttachementID, m_Specification.Samples, GL_DEPTH24_STENCIL8,
                    GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.width, m_Specification.height);
                break;
            }
        }

        if (m_ColorAttachementsID.size() > 1)
        {
            HASSERT(m_ColorAttachementsID.size() <= 4,"We dont support more than 4 color attachement for the moment");
            GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers(m_ColorAttachementsID.size(), buffers);
        }
        else if (m_ColorAttachementsID.empty())
        {
            // Only depth-pass
            glDrawBuffer(GL_NONE);
        }

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