#pragma once
#include <cstdint>


namespace TheEngine::RenderingSystem
{


    enum class ResourceFormat
    {
        UNKNOWN = 0,

        // --- Standard Unorm Formats (0.0 to 1.0) ---

        R8_UNORM,           // VK_FORMAT_R8_UNORM           | GL_R8, GL_RED, GL_UNSIGNED_BYTE
        RG8_UNORM,          // VK_FORMAT_R8G8_UNORM         | GL_RG8, GL_RG, GL_UNSIGNED_BYTE
        RGBA8_UNORM,        // VK_FORMAT_R8G8B8A8_UNORM     | GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE
        BGRA8_UNORM,        // VK_FORMAT_B8G8R8A8_UNORM     | GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE

        RG32_FLOAT,    // For vec2 (TexCoords)
        RGB32_FLOAT,   // For vec3 (Positions/Normals)


        // --- SRGB Formats (Gamma Corrected) ---

        RGBA8_SRGB,         // VK_FORMAT_R8G8B8A8_SRGB      | GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE


        // --- Floating Point Formats (HDR) ---

        R16_FLOAT,          // VK_FORMAT_R16_SFLOAT         | GL_R16F, GL_RED, GL_HALF_FLOAT
        RG16_FLOAT,         // VK_FORMAT_R16G16_SFLOAT       | GL_RG16F, GL_RG, GL_HALF_FLOAT
        RGBA16_FLOAT,       // VK_FORMAT_R16G16B16A16_SFLOAT | GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT

        R32_FLOAT,          // VK_FORMAT_R32_SFLOAT         | GL_R32F, GL_RED, GL_FLOAT
        RGBA32_FLOAT,       // VK_FORMAT_R32G32B32A32_SFLOAT | GL_RGBA32F, GL_RGBA, GL_FLOAT

        // --- Special Packed Formats ---

        R11G11B10_FLOAT,    // VK_FORMAT_B10G11R11_UFLOAT_PACK32 | GL_R11F_G11F_B10F, GL_RGB, GL_UNSIGNED_INT_10F_11F_11F_REV
        RGB10A2_UNORM,      // VK_FORMAT_A2B10G11R11_UNORM_PACK32| GL_RGB10_A2, GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV

        // --- Integer Formats (For Picking/ID buffers) ---

        R32_UINT,           // VK_FORMAT_R32_UINT           | GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT
        RGBA32_UINT,        // VK_FORMAT_R32G32B32A32_UINT  | GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT

        // --- Depth and Stencil ---

        D16_UNORM,          // VK_FORMAT_D16_UNORM          | GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT
        D32_FLOAT,          // VK_FORMAT_D32_SFLOAT         | GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT
        D24_S8_UINT,        // VK_FORMAT_D24_UNORM_S8_UINT  | GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8
        D32_S8_FLOAT,       // VK_FORMAT_D32_SFLOAT_S8_UINT | GL_DEPTH32F_STENCIL8, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV
    };



    enum class ShaderStageFlags : uint32_t
    {
        NONE = 0,
        VERTEX = 1 << 0, 
        FRAGMENT = 1 << 1, 
        COMPUTE = 1 << 2, 
        GEOMETRY = 1 << 3, 
        TESSELLATION_CONTROL = 1 << 4, 
        TESSELLATION_EVALUATION = 1 << 5, 

   
        ALL_GRAPHICS = VERTEX | FRAGMENT | GEOMETRY,
        //ALL = 0x7FFFFFFF


    };

    inline ShaderStageFlags operator|(ShaderStageFlags a, ShaderStageFlags b)
    {
        return static_cast<ShaderStageFlags>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }


    enum class ResourceType
    {
        UNKNOWN,
        SAMPLER,
        COMBINED_IMAGE_SAMPLER,
        SAMPLED_IMAGE,//No sampler
        STORAGE_IMAGE,
        UNIFORM_TEXEL_BUFFER,
        STORAGE_TEXEL_BUFFER,
        UNIFORM_BUFFER,
        STORAGE_BUFFER,
        UNIFORM_BUFFER_DYNAMIC,
        STORAGE_BUFFER_DYNAMIC,
        INPUT_ATTACHMENT,
        ACCELERATION_STRUCTURE
    };











    enum class PrimitiveTopology
    {
        TRIANGLES,
        LINES,
        POINTS,
        TRIANGLE_STRIP

    };





    enum class CullMode
    {
        NONE,
        BACK,
        FRONT,
        FRONT_AND_BACK
    };


    enum class FrontFace
    {
        CLOCKWISE,
        COUNTER_CLOCKWISE
    };


    enum class CompareOp 
    {
        NEVER,
        LESS,
        EQUAL,
        LESS_OR_EQUAL,
        GREATER,
        NOT_EQUAL,
        GREATER_OR_EQUAL,
        ALWAYS
    };



    enum class FillMode 
    {
        SOLID,
        WIREFRAME,
        POINT
    };

    enum class BlendFactor 
    {
        ZERO,
        ONE,
        SRC_COLOR,
        ONE_MINUS_SRC_COLOR,
        DST_COLOR,
        ONE_MINUS_DST_COLOR,
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA,
        DST_ALPHA,
        ONE_MINUS_DST_ALPHA
    };

    enum class BlendOp
    {
        ADD,
        SUBTRACT,
        REVERSE_SUBTRACT,
        MIN,
        MAX
    };






    enum class ColorComponentFlags : uint8_t
    {

        NONE = 0,
        R = 1 << 0,
        G = 1 << 1,
        B = 1 << 2,
        A = 1 << 3,
        ALL = R | G | B | A

 
    };








    enum class StencilOp
    {
        KEEP,      
        ZERO,    
        REPLACE,  
        INCREMENT_AND_CLAMP,
        DECREMENT_AND_CLAMP,
        INVERT,
        INCREMENT_AND_WRAP,
        DECREMENT_AND_WRAP
    };



    enum class SampleCountBit : uint32_t
    {
        SAMPLE_COUNT_1_BIT = 1 << 0,
        SAMPLE_COUNT_2_BIT = 1 << 1,
        SAMPLE_COUNT_4_BIT = 1 << 2,
        SAMPLE_COUNT_8_BIT = 1 << 3,
        SAMPLE_COUNT_16_BIT = 1 << 4,
        SAMPLE_COUNT_32_BIT = 1 << 5,
        SAMPLE_COUNT_64_BIT = 1 << 6
    };




    enum class AttachmentLoadOp
    {
        LOAD,      // VK_ATTACHMENT_LOAD_OP_LOAD      -> Keep existing contents
        CLEAR,     // VK_ATTACHMENT_LOAD_OP_CLEAR     -> Clear to a specific color/value
        DONT_CARE  // VK_ATTACHMENT_LOAD_OP_DONT_CARE -> Contents are undefined/discarded
    };

    enum class AttachmentStoreOp
    {
        STORE,     // VK_ATTACHMENT_STORE_OP_STORE     -> Write results to memory
        DONT_CARE, // VK_ATTACHMENT_STORE_OP_DONT_CARE -> Contents are not needed after the pass
        NONE       // VK_ATTACHMENT_STORE_OP_NONE      -> (Vulkan 1.3+) No store operation
    };


    enum class AttachmentResolveMode
    {
        NONE,           // VK_RESOLVE_MODE_NONE            -> No resolve operation
        SAMPLE_ZERO,    // VK_RESOLVE_MODE_SAMPLE_ZERO_BIT -> Result is sample 0
        AVERAGE,        // VK_RESOLVE_MODE_AVERAGE_BIT     -> Result is the average of samples
        MIN,            // VK_RESOLVE_MODE_MIN_BIT         -> Result is the minimum of samples
        MAX             // VK_RESOLVE_MODE_MAX_BIT         -> Result is the maximum of samples
    };




}