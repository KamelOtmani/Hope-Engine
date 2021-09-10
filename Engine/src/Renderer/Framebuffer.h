#pragma once

#include "Core/Core.h"
#include "HTypes.h"

namespace HEngine {
enum class FBTextureFormat {
    None = 0,

    // Color
    RGBA8,
    RGBA16F,

    // Depth
    DEPTH24STENCIL8,
    Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecefication {
    FramebufferTextureSpecefication() = default;
    FramebufferTextureSpecefication(FBTextureFormat format)
        : textureFormat(format)
    {
    }

    FramebufferTextureSpecefication(FBTextureFormat format, Vec4 clearColor)
        : textureFormat(format)
        , ClearColor(clearColor)
    {
    }
    FBTextureFormat textureFormat = FBTextureFormat::None;
    Vec4 ClearColor = Vec4(0.0f);
};

struct FramebufferAttachementSpecefication {
    FramebufferAttachementSpecefication() = default;
    FramebufferAttachementSpecefication(std::initializer_list<FramebufferTextureSpecefication> list)
        : Textures(list)
    {
    }
    Vector<FramebufferTextureSpecefication> Textures;
};

struct FramebufferSpecification {
    uint32_t width, height;
    FramebufferAttachementSpecefication Attachements;
    int Samples = 1;
};

class Framebuffer {
public:
    virtual ~Framebuffer() = default;

    virtual void Bind() = 0;
    virtual void UnBind() = 0;

    virtual void ClearAttachment(uint32_t attachmentIndex) = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;

    virtual uint32_t getColorAttachement(uint32_t index = 0) const = 0;
    virtual FramebufferTextureSpecefication& getAttachementSpecification(uint32_t index = 0) = 0;
    virtual const FramebufferSpecification& getSpecification() const = 0;

    static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
};
}
