#pragma once
#include <rendering-system/rhi/data-structures/gpu_types.h>
#include <rendering-system/engine_handles.h>
#include <rendering-system/rhi/data-structures/vertex_layout.h>
#include <rendering-system/rhi/data-structures/descriptor_set_layout.h>
#include <array>


namespace TheEngine::RenderingSystem
{


   




    struct StencilFaceState
    {
        StencilOp failOp = StencilOp::KEEP;      
        StencilOp passOp = StencilOp::KEEP;      // If stencil AND depth test pass
        StencilOp depthFailOp = StencilOp::KEEP; // If stencil passes but depth fails
        CompareOp compareOp = CompareOp::ALWAYS;

        uint32_t compareMask = 0xFF;
        uint32_t writeMask = 0xFF;
        uint32_t reference = 0;
    };










    struct RenderTargetFormat
    {
        ResourceFormat colorFormat = ResourceFormat::RGBA8_UNORM;
       // bool alphaBlending = false;

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













    struct DepthStencilState
    {
        bool depthTestEnable = true;
        bool depthWriteEnable = true;
        CompareOp depthCompareOp = CompareOp::LESS;

        bool stencilTestEnable = false;
        StencilFaceState stencilFront; // Vulkan requires front/back separate
        StencilFaceState stencilBack;


    };



    struct RasterizerState
    {
        CullMode cullMode = CullMode::BACK;
        FillMode fillMode = FillMode::SOLID;
        FrontFace frontFace = FrontFace::COUNTER_CLOCKWISE; 

        float lineWidth = 1.0f; // Note: Only works in GL/VK if enabled


        bool depthBiasEnable = false;
        float depthBiasConstant = 0.0f;
        float depthBiasSlope = 0.0f;
        float depthBiasClamp = 0.0f;
    };




    struct ColorAttachmentBlendState
    {
        bool blendEnable = false;


        BlendFactor srcColorBlendFactor = BlendFactor::ONE;
        BlendFactor dstColorBlendFactor = BlendFactor::ZERO;
        BlendOp colorBlendOp = BlendOp::ADD;

  
        BlendFactor srcAlphaBlendFactor = BlendFactor::ONE;
        BlendFactor dstAlphaBlendFactor = BlendFactor::ZERO;
        BlendOp alphaBlendOp = BlendOp::ADD;

        ColorComponentFlags colorWriteMask = ColorComponentFlags::ALL;
    };




    struct MultisampleState
    {
        SampleCountBit rasterizationSamples = SampleCountBit::SAMPLE_COUNT_1_BIT;

    
        bool sampleShadingEnable = false;
        float minSampleShading = 1.0f; // 0.0 to 1.0

    
        bool alphaToCoverageEnable = false;
        bool alphaToOneEnable = false;

        //for custom MSAA patterns
        // uint32_t sampleMask = 0xFFFFFFFF; 
    };




    struct PipelineStateConfig
    {

        DepthStencilState depthStencilState{};


        RasterizerState rasterizerState{};

        MultisampleState multisampleState{};



        std::array<ColorAttachmentBlendState, 8> colorAttachmentBlendStates;
        uint8_t colorAttachmentBlendStateCount = 1;
        void addBlendState(const ColorAttachmentBlendState& state)
        {
            if (colorAttachmentBlendStateCount < 8)
            {
                colorAttachmentBlendStates[colorAttachmentBlendStateCount++] = state;
            }
        }


        PrimitiveTopology topology = PrimitiveTopology::TRIANGLES;
    };
















    struct PushConstantLayout
    {
        uint32_t offset = 0;
        uint32_t size = 128; //Minimum Guranteed size as per vulkan 1.3 spec
        ShaderStageFlags shaderStageFlags = ShaderStageFlags::VERTEX | ShaderStageFlags::FRAGMENT;
    };






    
    class PipelineLayout
    {

    private:

        std::vector<DescriptorSetLayout> m_descriptorSetLayouts;
        std::vector<PushConstantLayout> m_pushConstantLayouts;


    public:

        PipelineLayout() = default;


        void setDescriptorSetLayouts(std::vector<DescriptorSetLayout>& layouts)
        {
            m_descriptorSetLayouts = layouts;
        }

        void setPushConstantLayouts(std::vector<PushConstantLayout>& layouts)
        {
            m_pushConstantLayouts = layouts;
        }




        // Getters

        std::vector<DescriptorSetLayout>& getDescriptorSetLayouts()
        {
            return m_descriptorSetLayouts;
        }

        std::vector<PushConstantLayout>& getPushConstantLayouts() 
        {
            return m_pushConstantLayouts;
        }

  
    };





    struct PipelineStateCreateInfo
    {


        ShaderHandle shaderHandle;

        VertexLayout vertexLayout;

        PipelineLayout pipelineLayout;


        PipelineStateConfig pipelineStateConfig;

        //frame buffer equivalent
        RenderOutputConfiguration renderOutputConfiguration;
    };






}
