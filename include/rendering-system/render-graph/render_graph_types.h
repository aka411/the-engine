#pragma once
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>
#include <rendering-system/rhi/i_command_buffer.h>
#include <rendering-system/rhi/data-structures/gpu_pipeline_data_structures.h>
#include <functional>
#include <string>
#include <rendering-system/rendering_system_data_types.h>

namespace TheEngine::RenderingSystem
{



    class RenderGraphBuilder;
    class PipelineSystem;
    class ShaderSystem;


    class ICommandBuffer ;
    class GPUResourceSystem;
    class DrawCallBucket;

    struct RenderPassSetupContext
    {
   
        PipelineSystem& pipelineSystem;
        ShaderSystem& shaderSystem;
        WindowExtent windowExtent;
    };


    struct RenderPassExecuteContext
    {
        ICommandBuffer& cmd;
        GPUResourceSystem& gpuResourceSystem;
        const DrawCallBucket& drawCallBucket; // The data to actually render
        Camera camera;
        WindowExtent windowExtend;
    };






	using ExecuteFunc = std::function<void(RenderPassExecuteContext&)>;
	using SetupFunc = std::function<ExecuteFunc(RenderPassSetupContext&, RenderGraphBuilder&)>;




    /*
    struct RenderGraphNode
    {
        std::string m_name;
        uint32_t m_index; // position in the global list

        // Dependency Tracking
       // std::vector<> m_inputs;
       // std::vector<> m_outputs;


        // Execution Logic

        SetupFunc setupFunc;
        ExecuteFunc m_executeFunc;


    };
    */


    struct RenderReadWriteTargets
    {
        RenderTargetFormat renderTargetFormat{};
        TextureHandle textureHandle{};
    };





    /*
     auto execFunc = [](RenderGraphBuilder& renderGraphBuilder)
         {




             return [=](ICommandBuffer& commandBuffer)
                 {
                     //The Execute function



                 };

         };

         */



    /*
    
    namespace TheEngine::RenderingSystem {

    class IRenderPass {
    public:
        virtual ~IRenderPass() = default;

        // The Setup phase
        virtual void Setup(RenderPassSetupContext& ctx, RenderGraphBuilder& builder) = 0;

        // The Execute phase:
        virtual void Execute(RenderPassExecuteContext& ctx) = 0;
        
       
        virtual std::string GetName() const = 0;
    };
}



class IRenderNode
{
private:


public:

virtual void setUp(RenderPassSetupContext& ctx, RenderGraphBuilder& builder) = 0;
virtual void execute(RenderPassExecuteContext& ctx) = 0;

virtual void onResize(const WindowExtent& extent) = 0;
       
virtual std::string GetName() const = 0;

}




*/

}