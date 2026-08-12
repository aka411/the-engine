#pragma once
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>
#include <rendering-system/rhi/i_command_buffer.h>
#include <rendering-system/rhi/data-structures/gpu_pipeline_data_structures.h>
#include <functional>
#include <string>
#include <rendering-system/rendering_system_data_types.h>


#include <rendering-system/engine_handles.h>
#include <unordered_map>
#include <vector>

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
        RELATIVE_FRAMEBUFFER,
        MATCH_FRAMEBUFFER
    };

    struct ResizeParameters
    {
        ResizingMode resizingMode{ ResizingMode::MATCH_FRAMEBUFFER };
        float relativeMultiplier{ 1.0f };
    };

    struct RenderGraphTextureCreateInfo
    {
        ResizeParameters resizeParameters{};
        TextureCreateInfo textureCreateInfo{};
    };


    template<typename Tag>
    struct VirtualResourceHandle
    {
        size_t id{ 0 };
    };

    using VirtualTextureId = VirtualResourceHandle<struct VirtualTextureTag>;






    struct RenderGraphResources
    {
        std::unordered_map<std::string, VirtualTextureId> textureResources;
        //Index is virtual handle
        std::vector<RenderGraphTextureCreateInfo> textureCreateInfos;
        std::vector<TextureHandle> textureHandles;

        TextureHandle getTextureHandle(const VirtualTextureId virtualTextureId) const
        {
            assert(virtualTextureId.id < textureHandles.size());
            return textureHandles[virtualTextureId.id];
        }

        TextureHandle getTextureHandle(const std::string& name) const
        {
            const auto& vd = textureResources.at(name);
            return getTextureHandle(vd);
        }

    };



}