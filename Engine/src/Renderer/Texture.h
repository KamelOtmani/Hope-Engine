#pragma once

namespace HEngine
{
	enum class TEXTURE_FILTER
	{
		NEAREST,
		LINEAR
	};

    enum class TEXTURE_FORMAT
    {
        UKNOWN = 0,

        // Color
        RGBA8,
        RGBA16F,
            // 3 channels
        RGB8,
        RGB16F,
            // 2 channels
        RG8,
        RG16F,
            // 1 channel
        R8,
        R16F,

        // Depth
        DEPTH24STENCIL8
    };

    enum class TEXTURE_WRAP
    {
        REPEAT,
        CLAMP
    };

    enum TEXTURE_TYPE
    {
        TEXTURE_1D,
        TEXTURE_2D,
        TEXTURE_3D
    };


	struct TextureSpecefication
    {

        uint32_t width = 1;
        uint32_t height = 1;

        TEXTURE_FORMAT format = TEXTURE_FORMAT::RGBA8;

        TEXTURE_FILTER magnificationFilterType = TEXTURE_FILTER::LINEAR;
        TEXTURE_FILTER minificationFilterType = TEXTURE_FILTER::LINEAR;

		TEXTURE_WRAP wrapType = TEXTURE_WRAP::REPEAT;

		bool bMipMaps = false;
		uint32_t mipLevels = 1;


	};

	class Texture
	{
	public:
		virtual ~Texture() = default;

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;
        virtual uint32_t getID() const = 0;

        virtual void setData(void* data, uint32_t size) const = 0;
        virtual void UpdateSpecification() const = 0;
        virtual void Invalidate() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual bool operator==(const Texture& other) const = 0;

	};

	class Texture2D : public Texture
	{
	public:
        static Ref<Texture2D> Create(TextureSpecefication spec);
        static Ref<Texture2D> Create(const std::string& path, bool bHDR = false, bool bMipMaps= false);
        TextureSpecefication textureSpec;
	};
}

