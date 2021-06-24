#pragma once
#include "Renderer/Texture.h"

#include <glad/glad.h>

namespace HEngine
{
    class GLTexture2D :
        public Texture2D
    {
    public:
        GLTexture2D(TextureSpecefication spec);
        GLTexture2D(std::string path, bool bHDR,bool bMipMaps);
        virtual ~GLTexture2D();

        virtual uint32_t getWidth() const override { return textureSpec.width; };
        virtual uint32_t getHeight() const override { return textureSpec.height; };
        virtual uint32_t getID() const override { return m_ID; };

        virtual void setData(void* data, uint32_t size) const override;

        virtual void UpdateSpecification()const override;
        virtual void Invalidate()const override;

        virtual void Bind(uint32_t slot = 0) const override;

        virtual bool operator==(const Texture& other) const override;

    private:
        std::string m_Path;
        uint32_t m_ID;
        GLenum m_InternalFormat, m_DataFormat;
    };
}

