#pragma once

#include "Core/Core.h"

namespace HEngine
{
    namespace Utils {
        static std::pair<GLenum, GLenum> HopeFormatToOpenGL(TEXTURE_FORMAT format)
        {
            switch (format)
            {
            case HEngine::TEXTURE_FORMAT::UKNOWN:
                HASSERT(false, "UKNOWN texture format")
                    break;
            case HEngine::TEXTURE_FORMAT::RGBA8:
                return std::make_pair(GL_RGBA8, GL_RGBA);
                break;
            case HEngine::TEXTURE_FORMAT::RGBA16F:
                return std::make_pair(GL_RGBA16F, GL_RGBA);
                break;
            case HEngine::TEXTURE_FORMAT::DEPTH24STENCIL8:
                HASSERT(false, "DEPTH formats are not supported for texture format for the moment")
                    break;
            default:
                break;
            }
        }

        static GLenum HopeFilterToOpenGL(TEXTURE_FILTER filter, bool bUseMipMaps = false)
        {
            switch (filter)
            {
            case HEngine::TEXTURE_FILTER::NEAREST:
                return bUseMipMaps ? GL_NEAREST : GL_NEAREST_MIPMAP_NEAREST;
                break;
            case HEngine::TEXTURE_FILTER::LINEAR:
                return bUseMipMaps ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR;
                break;
            default:
                break;
            }
        }

        static GLenum HopeWrapToOpenGL(TEXTURE_WRAP wrap)
        {
            switch (wrap)
            {
            case HEngine::TEXTURE_WRAP::REPEAT:
                return GL_REPEAT;
                break;
            case HEngine::TEXTURE_WRAP::CLAMP:
                return GL_CLAMP_TO_BORDER;
                break;
            default:
                break;
            }
        }

        static GLenum HopeElementTypeToOpenGL(TEXTURE_FORMAT format)
        {
            switch (format)
            {
            case HEngine::TEXTURE_FORMAT::UKNOWN:
                HASSERT(false, "UKNOWN texture format")
                    break;
            case HEngine::TEXTURE_FORMAT::RGBA16F:
                return GL_FLOAT;
                break;
            case HEngine::TEXTURE_FORMAT::RGBA8:
            case HEngine::TEXTURE_FORMAT::DEPTH24STENCIL8:
                HASSERT(false, "DEPTH formats are not supported for texture format for the moment")
                    break;
            default:
                break;
            }
        }
    }
}