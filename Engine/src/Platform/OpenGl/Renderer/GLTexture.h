#pragma once
#include "Renderer/Texture.h"

#include <glad/glad.h>

namespace HEngine
{
    class GLTexture2D :
        public Texture2D
    {
    public:
        GLTexture2D(uint32_t width, uint32_t height);
        GLTexture2D(std::string path);
        virtual ~GLTexture2D();

        virtual uint32_t getWidth() const override { return m_Width; };
        virtual uint32_t getHeight() const override { return m_Height; };
        virtual uint32_t getID() const override { return m_ID; };

        virtual void setData(void* data, uint32_t size) const override;

        virtual void Bind(uint32_t slot = 0) const override;

        virtual bool operator==(const Texture& other) const override;

    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        uint32_t m_ID;
        GLenum m_InternalFormat, m_DataFormat;
    };
}

