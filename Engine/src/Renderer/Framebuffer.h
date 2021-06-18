#pragma once

#include "Core/Core.h"

namespace HEngine
{
    enum class FBTextureFormat
    {
        None = 0,

        // Color
        RGBA8,
        RGBA16F,

        // Depth
        DEPTH24STENCIL8,
        Depth = DEPTH24STENCIL8
    };


    struct FramebufferTextureSpecefication
    {
        FramebufferTextureSpecefication() = default;
        FramebufferTextureSpecefication(FBTextureFormat format)
            : textureFormat(format) {}
        FBTextureFormat textureFormat = FBTextureFormat::None;
    };

    struct FramebufferAttachementSpecefication
    {
        FramebufferAttachementSpecefication() = default;
        FramebufferAttachementSpecefication(std::initializer_list<FramebufferTextureSpecefication> list)
            : Textures(list) {}
        Vector<FramebufferTextureSpecefication> Textures;
    };


    struct FramebufferSpecification
    {
        uint32_t width, height;
        FramebufferAttachementSpecefication Attachements;
        int Samples = 1;
    };

    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        virtual void Bind() = 0;
        virtual void UnBind() = 0;
        
        virtual void Resize(uint32_t width, uint32_t height) = 0;

        virtual uint32_t getColorAttachement(uint32_t index = 0) const = 0;
        virtual const FramebufferSpecification& getSpecification() const = 0;

        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
    };
}

