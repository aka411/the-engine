#pragma once
#include "igpu_resource.h"
#include <cstdint>

//mostly cleaned for now can skip
namespace TheEngine::Graphics
{
    enum class TextureType
    {
		TEXTURE_1D, // Rarely used, often for specific data like height maps
        TEXTURE_2D,
        TEXTURE_3D,


    };

    enum class TextureFormat
    {
        // --- Unsigned Normalized Integer (UNORM) - Common for color, usually [0, 1] range ---
        R8_UNORM,
        R16_UNORM,
        R32_UNORM, // Not very common, often replaced by R32_FLOAT for precision
        RG8_UNORM,
        RG16_UNORM,
        RG32_UNORM, // Not very common
        RGB8_UNORM, // Less common on modern GPUs (often padded to RGBA)
        RGBA8_UNORM, // VERY COMMON - Default 8-bit color
        RGBA16_UNORM, // Common for higher quality color or data
        RGB10A2_UNORM, // Common for HDR or higher precision rendering
        BGRA8_UNORM, // Common on Windows/DirectX platforms, good for swapchains

        // --- Signed Normalized Integer (SNORM) - Common for normal maps, usually [-1, 1] range ---
        R8_SNORM,
        RG8_SNORM,
        RGB8_SNORM, // Less common
        RGBA8_SNORM,
        RG16_SNORM,
        RGBA16_SNORM,

        // --- Unsigned Integer (UINT) - Stores raw integer values ---
        R8_UINT,
        R16_UINT,
        R32_UINT,
        RG8_UINT,
        RG16_UINT,
        RG32_UINT,
        RGBA8_UINT,
        RGBA16_UINT,
        RGBA32_UINT,

        // --- Signed Integer (SINT) - Stores raw integer values ---
        R8_SINT,
        R16_SINT,
        R32_SINT,
        RG8_SINT,
        RG16_SINT,
        RG32_SINT,
        RGBA8_SINT,
        RGBA16_SINT,
        RGBA32_SINT,

        // --- Floating Point (FLOAT) - High precision, wide range ---
        R16_FLOAT,
        R32_FLOAT, // Common for single-channel HDR, height maps, or data
        RG16_FLOAT,
        RG32_FLOAT,
        RGB16_FLOAT,
        RGB32_FLOAT,
        RGBA16_FLOAT, // Common for HDR rendering, often preferred over RGB16_FLOAT
        RGBA32_FLOAT, // Common for G-buffers, high precision calculations

        // --- sRGB (Gamma-corrected) Formats - For display output or color textures ---
        RGBA8_SRGB, // VERY COMMON for color textures intended for display
        BGRA8_SRGB, // Common for swapchains on Windows

        // --- Depth and Stencil Formats ---
        D16_UNORM,
        D32_FLOAT,

        D24_UNORM_S8_UINT, // VERY COMMON - Depth with Stencil
        D32_FLOAT_S8_UINT, // Depth with Stencil (higher precision depth)



        UNKNOWN = 0 // Always good to have an unknown/error state
    };


    enum class TextureUsageFlags : uint32_t
    {
        /**
    * @brief No usage specified.
    */
        NONE = 0,

        /**
         * @brief Texture can be sampled by a shader (e.g., via `sampler2D` in GLSL).
         * Corresponds to `VK_IMAGE_USAGE_SAMPLED_BIT`.
         */
        SAMPLED = 1 << 0,

        /**
         * @brief Texture can be used as a color attachment in a render pass.
         * Corresponds to `VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT`.
         */
        COLOR_ATTACHMENT = 1 << 1,

        /**
         * @brief Texture can be used as a depth or stencil attachment in a render pass.
         * Corresponds to `VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT`.
         */
        DEPTH_STENCIL_ATTACHMENT = 1 << 2,

        /**
         * @brief Texture can be used as a source for transfer operations (e.g., `vkCmdCopyImage`, `vkCmdBlitImage`).
         * This means it can be copied FROM.
         * Corresponds to `VK_IMAGE_USAGE_TRANSFER_SRC_BIT`.
         */
        TRANSFER_SRC = 1 << 3,

        /**
         * @brief Texture can be used as a destination for transfer operations (e.g., `vkCmdCopyBufferToImage`, `vkCmdBlitImage`).
         * This means data can be copied TO it.
         * Corresponds to `VK_IMAGE_USAGE_TRANSFER_DST_BIT`.
         */
        TRANSFER_DST = 1 << 4,

        /**
         * @brief Texture can be used as a storage image (read/write access) by a shader.
         * Typically for compute shaders or advanced graphics techniques.
         * Corresponds to `VK_IMAGE_USAGE_STORAGE_BIT`.
         */
        STORAGE = 1 << 5,

        /**
         * @brief Texture can be used as an input attachment in a subpass.
         * This is primarily a Vulkan-specific concept for reading from an attachment
         * from a previous subpass within the same render pass.
         * Corresponds to `VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT`.
         */
        INPUT_ATTACHMENT = 1 << 6,

        /**
         * @brief Texture is a transient attachment; its contents may only be needed within a render pass.
         * This hints to the driver that memory for the attachment can be allocated from transient (fast) memory,
         * and its contents don't need to be preserved after the render pass ends.
         * Corresponds to `VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT`.
         */
        TRANSIENT_ATTACHMENT = 1 << 7,



    };



    /*
    VkImage image;
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = textureWidth;
    imageInfo.extent.height = textureHeight;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = numMipLevels; // Calculated based on texture dimensions
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT; // We'll copy data TO it, then sample FROM it
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0;
    */
    struct TextureCreateInfo 
    {
        TextureType type;
		TextureFormat format; //consider name pixelFormat or textureFormat for clarity

        uint32_t width;
        uint32_t height;

        
        uint32_t mipLevels = 1;

        TextureUsageFlags usageFlags; // e.g., Sampled, ColorAttachment, DepthAttachment, TransferSrc, TransferDst



    };






	class IGPUTexture : public IGPUResource
    {
    public:
        virtual ~IGPUTexture() = default;

       // ResourceType getResourceType() const override { return ResourceType::TEXTURE; }

        // Getters for properties set at creation
        virtual TextureType GetType() const = 0;
        virtual TextureFormat GetFormat() const = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;


        virtual uint32_t GetMipLevels() const = 0;

        virtual TextureUsageFlags GetUsage() const = 0;

       

    };




}