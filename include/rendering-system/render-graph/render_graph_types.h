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




    struct RenderPassSetupContext
    {
   
        PipelineSystem& pipelineSystem;
        ShaderSystem& shaderSystem;

        WindowExtent windowExtent;
    };


    struct RenderPassExecuteContext
    {
        ICommandBuffer& cmd;
        UserPassData userPassData{};
        WindowExtent windowExtend{};
    };






	using ExecuteFunc = std::function<void(RenderPassExecuteContext&)>;
	using SetupFunc = std::function<ExecuteFunc(RenderPassSetupContext&, RenderGraphBuilder&)>;


    enum class ResizingMode
    {
        ABSOLUTE,
        RELATIVE
    };

    struct ResizeParameters
    {
        ResizingMode resizingMode{ ResizingMode::ABSOLUTE};
        float relativeMultiplier{ 1.0f };
    };


}