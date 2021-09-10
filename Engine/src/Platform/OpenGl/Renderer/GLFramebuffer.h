#pragma once

#include "Renderer/Framebuffer.h"
#include <glad/glad.h>

namespace HEngine {
class GLFramebuffer : public Framebuffer {
public:
    GLFramebuffer(const FramebufferSpecification& spec);
    ~GLFramebuffer();

    void Invalidate();

    void Bind() override;
    void UnBind() override;

    void Resize(uint32_t width, uint32_t height) override;

    void ClearAttachment(uint32_t attachmentIndex) override;

    uint32_t getColorAttachement(uint32_t index = 0) const override { return m_ColorAttachementsID[index]; };
    const FramebufferSpecification& getSpecification() const override { return m_Specification; };
    FramebufferTextureSpecefication& getAttachementSpecification(uint32_t index = 0) override { return m_ColorAttachmentSpecifications[index]; };

private:
    Vector<FramebufferTextureSpecefication> m_ColorAttachmentSpecifications;
    FramebufferTextureSpecefication m_DepthAttachmentSpecification = FBTextureFormat::None;

    Vector<uint32_t> m_ColorAttachementsID;
    uint32_t m_DepthAttachementID = 0;

    uint32_t m_ID = 0;
    FramebufferSpecification m_Specification;
};

}
