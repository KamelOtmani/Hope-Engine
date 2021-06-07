#pragma once


namespace HEngine
{
    struct FramebufferSpecification
    {
        uint32_t width, height;
    };
    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

        virtual void Bind() = 0;
        virtual void UnBind() = 0;
        
        virtual void Resize(uint32_t width, uint32_t height) = 0;

        virtual uint32_t getColorAttachement() const = 0;
        virtual const FramebufferSpecification& getSpecification() const = 0;

        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
    };
}

