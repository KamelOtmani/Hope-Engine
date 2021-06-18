#pragma once

#include "Renderer/Framebuffer.h"
#include <glad/glad.h>

namespace HEngine
{
	class GLFramebuffer : public Framebuffer
	{
	public:
		GLFramebuffer(const FramebufferSpecification& spec);
        ~GLFramebuffer();

        void Invalidate();

        virtual void Bind() override;
        virtual void UnBind() override;

        virtual void Resize(uint32_t width, uint32_t height) override;


        virtual uint32_t getColorAttachement(uint32_t index = 0) const override { return m_ColorAttachementsID[index]; };
        virtual const FramebufferSpecification& getSpecification() const override { return m_Specification; };

	private:

        Vector<FramebufferTextureSpecefication> m_ColorAttachmentSpecifications;
        FramebufferTextureSpecefication m_DepthAttachmentSpecification = FBTextureFormat::None;

        Vector<uint32_t> m_ColorAttachementsID;
        uint32_t m_DepthAttachementID;

        uint32_t m_ID;
		FramebufferSpecification m_Specification;
	};

}
