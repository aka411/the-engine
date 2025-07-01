#pragma once


#include <vector>
#include <memory>
#include "igpu_resource.h"

#include "core/vertex_input_layout.h"
#include "igpu_shader_module.h"



//need more cleaning , clean during implementing time
namespace TheEngine::Graphics
{
	enum class PolygonMode
	{
		FILL,          // Solid fill
		LINE,          // Wireframe
		POINT          // Points
	};

    enum class CullMode
    {
		NONE,         // No culling
		FRONT,        // Cull front faces
		BACK,         // Cull back faces
		FRONT_AND_BACK  // Cull both front and back faces
    };

	enum class FrontFace
	{
		CLOCKWISE,         // Clockwise winding order
		COUNTERCLOCKWISE   // Counter-clockwise winding order (often default)
	};

	enum class CompareOp
	{
		Never,
		Less,
		Equal,
		LessOrEqual,
		Greater,
		NotEqual,
		GreaterOrEqual,
		Always
	};

    enum class BlendFactor
    {
        Zero,
        One,
        Src_Color,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        ConstantColor, // For constant blend color
        OneMinusConstantColor,
        ConstantAlpha,
        OneMinusConstantAlpha
    };

	enum class BlendOp
	{
		ADD,
		SUBTRACT,
		REVERSE_SUBTRACT,
		MIN, // Minimum of source and destination
		MAX  // Maximum of source and destination
	};

    enum class ColorWriteMask
    {
		NONE = 0,          // No color write
		RED = 1 << 0,      // Write red channel
		GREEN = 1 << 1,    // Write green channel
		BLUE = 1 << 2,     // Write blue channel
		ALPHA = 1 << 3,    // Write alpha channel
		ALL = RED | GREEN | BLUE | ALPHA // Write all channels
    };



    // API-agnostic Input Assembly State
    struct InputAssemblyState
    {
        PrimitiveTopology topology;
        bool primitiveRestartEnable = false;
    };





	struct RasterizationStateCreateInfo
    {
        PolygonMode polygonMode = PolygonMode::Fill;
        CullMode cullMode = CullMode::Back;
        FrontFace frontFace = FrontFace::CounterClockwise; // Often default to CCW
        float lineWidth = 1.0f;
        bool depthClampEnable = false;
        bool rasterizerDiscardEnable = false;
        bool depthBiasEnable = false;
        float depthBiasConstantFactor = 0.0f;
        float depthBiasClamp = 0.0f;
        float depthBiasSlopeFactor = 0.0f;
    };

    // Viewport struct
    struct Viewport
    {
        float x, y, width, height, minDepth, maxDepth;
    };

    // Scissor struct
    struct Scissor
    {
        int32_t x, y, width, height;
    };


    // API-agnostic Shader Stage Information
	struct PipelineShaderStageCreateInfo//need more designing
    {
        ShaderStage stage;
        IGPUShaderModule* pShaderModule; // Pointer to an abstract shader module handle
        std::string entryPoint = "main"; // Entry point function name
        // Could add specialization constants here if needed
    };




    // API-agnostic Multisample State
	struct MultisampleStateCreateInfo
    {
        uint32_t rasterizationSamples = 1; // 1 means no MSAA, corresponds to VK_SAMPLE_COUNT_1_BIT
        bool sampleShadingEnable = false;
        float minSampleShading = 0.0f; // Ignored if sampleShadingEnable is false
        // uint32_t* pSampleMask = nullptr; // Complex, might omit for simplicity or encapsulate
        bool alphaToCoverageEnable = false;
        bool alphaToOneEnable = false;
    };



    // API-agnostic Depth Stencil State
	struct DepthStencilStateCreateInfo
    {
        bool depthTestEnable = true;
        bool depthWriteEnable = true;
        CompareOp depthCompareOp = CompareOp::Less;
        bool depthBoundsTestEnable = false;
        float minDepthBounds = 0.0f;
        float maxDepthBounds = 1.0f;
        bool stencilTestEnable = false;

    };

    // API-agnostic Color Blend Attachment State (per render target)
	struct ColorBlendAttachmentState
    {
        bool blendEnable = false;
        BlendFactor srcColorBlendFactor = BlendFactor::One;
        BlendFactor dstColorBlendFactor = BlendFactor::Zero;
        BlendOp colorBlendOp = BlendOp::Add;
        BlendFactor srcAlphaBlendFactor = BlendFactor::One;
        BlendFactor dstAlphaBlendFactor = BlendFactor::Zero;
        BlendOp alphaBlendOp = BlendOp::Add;
        ColorWriteMask colorWriteMask = ColorWriteMask::All;
    };


    // API-agnostic Color Blend State (global)
	struct ColorBlendStateCreateInfo
    {
        bool logicOpEnable = false;
        // LogicOp logicOp; // Need to define LogicOp enum if used
        std::vector<ColorBlendAttachmentState> attachments;
        float blendConstants[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    };


    // API-agnostic Dynamic States (states that can be changed after pipeline creation)
    enum class DynamicState
    {
        VIEWPORT,
        SCISSOR,
        LINE_WIDTH,
        DEPTH_BIAS,
        BLEND_CONSTANTS,
        DEPTH_BOUNDS,
        STENCIL_COMPARE_MASK,
        STENCIL_WRITE_MASK,
        STENCIL_REFERENCE,
        // Add more as supported by underlying APIs and your engine's needs
    };




    // --- Graphics Pipeline Creation Info ---
    struct PipelineCreateInfo
    {
        IGPUPipelineLayout* pLayout = nullptr; // The pipeline layout for resources

        // Shader Stages
        std::vector<PipelineShaderStageCreateInfo> shaderStages;//need moe designing


        // Vertex Input State
        VertexInputLayout vertexInputLayout; // From previous discussion

        // Input Assembly State
		InputAssemblyState inputAssemblyState;
 
        // Viewport and Scissor (Can be fixed or dynamic)
        uint32_t viewportCount = 1;
        std::vector<Viewport> viewports;

        uint32_t scissorCount = 1; // Typically matches viewportCount
        std::vector<Scissor> scissors;

        // Rasterization State
		RasterizationStateCreateInfo rasterizationState;

        // Multisample State
        MultisampleStateCreateInfo multisampleState;

        // Depth/Stencil State
        DepthStencilStateCreateInfo depthStencilState;

        // Color Blend State
        ColorBlendStateCreateInfo colorBlendState;

        // Dynamic States (states that can be set by command buffer, not fixed in pipeline)
        std::vector<DynamicState> dynamicStates;

        // Render Pass Compatibility (Vulkan requires this, OpenGL implicitly uses current FBO state)
        // You might use a RenderPassInfo struct here to define the formats and attachment counts
        // this pipeline expects to be compatible with, or an actual IGPURenderPass* if you abstract RenderPasses.
        // Using RenderPassInfo for explicit compatibility:
        RenderPassInfo renderPassInfo; // Defines compatible formats, attachment counts, subpasses
        uint32_t subpassIndex = 0; // Which subpass in the RenderPassInfo this pipeline is for

        /*

         // Pipeline Derivation (for creating pipelines from existing ones)
        IGPUPipeline* pBasePipeline = nullptr; // Existing pipeline to derive from
        int32_t basePipelineIndex = -1;      // Or an index into an array of pipelines


        // Optional: Pipeline cache for faster creation
        // IGPUPipelineCache* pPipelineCache = nullptr;
        */
    };

    // --- Compute Pipeline Creation Info ---
    struct ComputePipelineCreateInfo
    {
        IGPUPipelineLayout* pLayout = nullptr; // The pipeline layout for resources
        IGPUShaderModule* pComputeShader = nullptr; // The compute shader module
    };







    class IGPUPipelineState : public IGPUResource
    {
    private:
        // Private constructor to prevent direct instantiation.
        // Pipeline states are created via the IGPURenderDevice.
        IGPUPipelineState() = delete;

    public:
        // Virtual destructor for proper polymorphic cleanup.
        virtual ~IGPUPipelineState() = default;

        // No public virtual methods are typically needed here, as the pipeline state
        // is simply bound to a command buffer. All its configuration is done at creation.
        // Implementations will likely have internal methods or properties.
    };


} // namespace Engine