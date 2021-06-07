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


        virtual uint32_t getColorAttachement() const override { return m_ColorAttachementID; };
        virtual const FramebufferSpecification& getSpecification() const override { return m_Specification; };

	private:
        uint32_t m_ColorAttachementID;
        uint32_t m_DepthAttachementID;
        uint32_t m_ID;
		FramebufferSpecification m_Specification;
	};

}
