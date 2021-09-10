#include "hpch.h"
#include "GLTexture.h"

#include <glad/glad.h>

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <Core\Core.h>

#include "Platform/OpenGl/GLUtility.h"

namespace HEngine {

GLTexture2D::GLTexture2D(TextureSpecefication spec)
{
    textureSpec = spec;

    auto& [m_InternalFormat, m_DataFormat] = Utils::HopeFormatToOpenGL(spec.format);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
    Invalidate();
}

GLTexture2D::~GLTexture2D()
{
    glDeleteTextures(1, &m_ID);
}

void GLTexture2D::setData(void* data, uint32_t size) const
{
    auto& [m_InternalFormat, m_DataFormat] = Utils::HopeFormatToOpenGL(textureSpec.format);
    auto type = Utils::HopeElementTypeToOpenGL(textureSpec.format);
    glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, textureSpec.width, textureSpec.height, 0, m_DataFormat, GL_FLOAT, data);
}

void GLTexture2D::UpdateSpecification() const
{

    glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, Utils::HopeFilterToOpenGL(textureSpec.minificationFilterType, textureSpec.bMipMaps));
    glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, Utils::HopeFilterToOpenGL(textureSpec.magnificationFilterType, textureSpec.bMipMaps));

    glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, Utils::HopeWrapToOpenGL(textureSpec.wrapType));
    glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, Utils::HopeWrapToOpenGL(textureSpec.wrapType));
}

void GLTexture2D::Invalidate() const
{
    glTextureStorage2D(m_ID, textureSpec.bMipMaps ? textureSpec.mipLevels : 1, m_InternalFormat, textureSpec.width, textureSpec.height);
    UpdateSpecification();
}

void GLTexture2D::Bind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

bool GLTexture2D::operator==(const Texture& other) const
{
    return m_ID == ((GLTexture2D&)other).getID();
}

GLTexture2D::GLTexture2D(std::string path, bool bHDR, bool bMipMaps)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    void* data = nullptr;
    GLenum internalFormat = 0, dataFormat = 0;
    if (bHDR) {
        {
            data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
        }
        HASSERT(data, "Failed to load image!");
        textureSpec.width = width;
        textureSpec.height = height;

        switch (channels) {
        case 4: {
            textureSpec.format = TEXTURE_FORMAT::RGBA16F;
            internalFormat = GL_RGBA16F;
            dataFormat = GL_RGBA;
            break;
        }
        case 3: {
            textureSpec.format = TEXTURE_FORMAT::RGB16F;
            internalFormat = GL_RGB16F;
            dataFormat = GL_RGB;
            break;
        }
        case 2: {
            textureSpec.format = TEXTURE_FORMAT::RG16F;
            internalFormat = GL_RG16F;
            dataFormat = GL_RG;
            break;
        }
        case 1: {
            textureSpec.format = TEXTURE_FORMAT::R16F;
            internalFormat = GL_R16F;
            dataFormat = GL_RED;
            break;
        }
        default:
            break;
        }

        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;
        textureSpec.bMipMaps = bMipMaps;
        textureSpec.mipLevels = bMipMaps ? 7 : 1;

        HASSERT(internalFormat & dataFormat, "Format not supported!");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
        glTextureStorage2D(m_ID, textureSpec.bMipMaps ? textureSpec.mipLevels : 1, internalFormat, textureSpec.width, textureSpec.height);
        glGenerateTextureMipmap(m_ID);

        UpdateSpecification();
        glTextureSubImage2D(m_ID, 0, 0, 0, textureSpec.width, textureSpec.height, dataFormat, GL_FLOAT, data);
        glGenerateTextureMipmap(m_ID);

    } else {
        {
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        }
        HASSERT(data, "Failed to load image!");
        textureSpec.width = width;
        textureSpec.height = height;

        switch (channels) {
        case 4: {
            textureSpec.format = TEXTURE_FORMAT::RGBA16F;
            internalFormat = GL_RGBA16F;
            dataFormat = GL_RGBA;
            break;
        }
        case 3: {
            textureSpec.format = TEXTURE_FORMAT::RGB16F;
            internalFormat = GL_RGB16F;
            dataFormat = GL_RGB;
            break;
        }
        case 2: {
            textureSpec.format = TEXTURE_FORMAT::RG16F;
            internalFormat = GL_RG16F;
            dataFormat = GL_RG;
            break;
        }
        case 1: {
            textureSpec.format = TEXTURE_FORMAT::R16F;
            internalFormat = GL_R16F;
            dataFormat = GL_RED;
            break;
        }
        default:
            break;
        }

        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;
        textureSpec.bMipMaps = bMipMaps;
        textureSpec.mipLevels = bMipMaps ? 7 : 1;

        HASSERT(internalFormat & dataFormat, "Format not supported!");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
        glTextureStorage2D(m_ID, textureSpec.bMipMaps ? textureSpec.mipLevels : 1, internalFormat, textureSpec.width, textureSpec.height);
        glGenerateTextureMipmap(m_ID);

        UpdateSpecification();
        glTextureSubImage2D(m_ID, 0, 0, 0, textureSpec.width, textureSpec.height, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(m_ID);
    }

    stbi_image_free(data);
}

}
