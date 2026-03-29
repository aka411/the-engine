#pragma once


namespace TheEngine::RenderingSystem
{


    //used to create the pipeline state object





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



    enum class CompareOp {
        NEVER,
        LESS,
        EQUAL,
        LESS_EQUAL,
        GREATER,
        NOT_EQUAL,
        GREATER_EQUAL,
        ALWAYS
    };

    enum class FillMode {
        SOLID,
        WIREFRAME,
        POINT
    };

    enum class BlendFactor {
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

    enum class BlendOp {
        ADD,
        SUBTRACT,
        REVERSE_SUBTRACT,
        MIN,
        MAX
    };


    // --- Sub-structs for organization ---
    struct DepthStencilState {
        bool depthTestEnable = true;
        bool depthWriteEnable = true;
        CompareOp depthCompareOp = CompareOp::LESS;

        // Missing: Stencil Ops
        bool stencilTestEnable = false;
        // StencilOp frontFail, frontPass, etc...


        bool depthBiasEnable = false;
        float depthBiasConstant = 0.0f;
        float depthBiasSlope = 0.0f;
    };

    struct RasterizerState {
        CullMode cullMode = CullMode::BACK;
        FillMode fillMode = FillMode::SOLID;
        //float lineWidth = 1.0f;
    };

    struct BlendState {
        bool enabled = false;
        BlendFactor srcColorFactor = BlendFactor::SRC_ALPHA;
        BlendFactor dstColorFactor = BlendFactor::ONE_MINUS_SRC_ALPHA;
        BlendOp colorBlendOp = BlendOp::ADD;

        uint8_t colorWriteMask = 0xF; // 0xF = Write all RGBA
    };




    enum ColorComponentFlags : uint8_t
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
        KEEP,      // Keep current value
        ZERO,      // Set to 0
        REPLACE,   // Replace with reference value
        INCREMENT_AND_CLAMP,
        DECREMENT_AND_CLAMP,
        INVERT,
        INCREMENT_AND_WRAP,
        DECREMENT_AND_WRAP
    };

    struct StencilFaceState
    {
        StencilOp failOp = StencilOp::KEEP;      // If stencil test fails
        StencilOp passOp = StencilOp::KEEP;      // If stencil AND depth test pass
        StencilOp depthFailOp = StencilOp::KEEP; // If stencil passes but depth fails
        CompareOp compareOp = CompareOp::ALWAYS;
        uint32_t compareMask = 0xFF;
        uint32_t writeMask = 0xFF;
        uint32_t reference = 0;
    };










    struct RenderTargetFormat
    {
        TextureFormat colorFormat = TextureFormat::RGBA8_UNORM;
        bool alphaBlending = false;

        //to handle resize
        float scaleX = 1.0f; // 1.0 = Full Window Width
        float scaleY = 1.0f;
        bool sizeRelativeToWindow = true;

        uint32_t sampleCount = 1;
    };

    struct RenderOutputConfiguration
    {
        // The index in this vector = The Attachment Slot (0, 1, 2...)
        std::vector<RenderTargetFormat> colorAttachments;
        RenderTargetFormat depthStencilAttachment;
        bool hasDepth = false;
        bool hasStencil = false;
    };







    struct RenderStateConfiguration {



        ShaderProgramHandle shader;



        VertexLayout vertexLayout;

        // Resource & Constant Layout
        std::vector<ResourceGroupLayout> resourceLayouts;//vector index not setIndex has explicit setIndex inside
        std::vector<ConstantRange> constantRanges;



        // Depth & Stencil
        struct DepthStencilState {
            bool depthTestEnable = true;
            bool depthWriteEnable = true;
            CompareOp depthCompareOp = CompareOp::LESS;

            bool stencilTestEnable = false;
            StencilFaceState stencilFront; // Vulkan requires front/back separate
            StencilFaceState stencilBack;

            // Depth Bias (Prevents shadow acne)
            bool depthBiasEnable = false;
            float depthBiasConstant = 0.0f;
            float depthBiasSlope = 0.0f;
        } depthStencil;

        // Rasterizer
        struct RasterizerState {
            CullMode cullMode = CullMode::BACK;
            FillMode fillMode = FillMode::SOLID;
            float lineWidth = 1.0f; // Note: Only works in GL/VK if enabled
        } rasterizer;

        // Blend
        struct BlendState {
            bool enabled = false;
            BlendFactor srcColorFactor = BlendFactor::SRC_ALPHA;
            BlendFactor dstColorFactor = BlendFactor::ONE_MINUS_SRC_ALPHA;
            BlendOp colorBlendOp = BlendOp::ADD;
            uint8_t colorWriteMask = ColorComponentFlags::ALL;

        } blend;

        PrimitiveTopology topology = PrimitiveTopology::TRIANGLES;
        uint32_t msaaSamples = 1; // 1, 2, 4, 8



        //frame buffer equivalent
        RenderOutputConfiguration output;
    };

    //push constants
    struct ConstantRange
    {
        uint32_t offset;
        uint32_t size;
        ShaderStage stage; // VERTEX, FRAGMENT, etc.
    };



    enum class ResourceType {
        UniformBuffer,
        StorageBuffer,
        ImageSampler,
        InputAttachment
    };
    enum class ShaderStage {
        Vertex = 1 << 0,
        Fragment = 1 << 1,
        Compute = 1 << 2,
        All = 0x7FFFFFFF
    };

    struct ResourceBinding {
        uint32_t bindingSlot;
        ResourceType type;
        ShaderStage stageFlags;

        // Only used for Storage Images or Input Attachments
        TextureFormat format = TextureFormat::Unknown;//not used for ImageSampler,

        // For Arrays (e.g., Texture myTextures[4])
        uint32_t descriptorCount = 1;
    };

    struct ResourceGroupLayout {
        uint32_t setIndex; // Set 0, Set 1, etc.
        std::vector<ResourceBinding> bindings;
    };


    //Pasout engine handles maybe?
    //Vulkan has a 1 to 1
    class IPipelineStateObject
    {

    private:

        RenderStateConfiguration m_renderStateConfiguration;


    public:



        IPipelineStateObject(RenderStateConfiguration renderStateConfiguration) :
            m_renderStateConfiguration(renderStateConfiguration)
        {

        }

        virtual ~IPipelineStateObject() = default;

        RenderStateConfiguration getRenderStateConfiguration()
        {
            return m_renderStateConfiguration;
        }

        //Native methods mostly

        //no need for bind maybe? let icommand buffer do it
        //remove
        //virtual void bind();




    };


}
