#pragma once


#include <rendering-system/render-graph/render_graph_types.h>
#include <rendering-system/render-graph/render_graph_builder.h>
#include <rendering-system/render-graph/i_render_graph_node.h>
#include <rendering-system/render-graph/gpu_resource_resolver.h>



namespace TheEngine::RenderingSystem
{


    class IRenderDevice;
    class ITextureManager;

    struct RenderPassSetupContext;
    struct RenderPassExecuteContext;



	class RenderGraph
	{

    private:

        ITextureManager& m_textureManager;
        RenderGraphResources m_renderGraphResources;

	private:

    
		std::vector<std::unique_ptr<IRenderGraphNode>> m_renderGraphNodes;

    private:

        RenderGraphBuilder m_renderGraphBuilder;

        RenderPassSetupContext m_renderPassSetupContext;

    private:

        GPUResourceResolver m_gpuResourceResolver;

    protected:

        friend class RenderingSystem;
        void onWindowResize(const WindowExtent& extent);

	public:

		RenderGraph(IRenderDevice& renderDevice, const RenderPassSetupContext& renderPassSetupContext,GPUResourceSystem& gpuResourceSystem);



        void addPass(std::unique_ptr<IRenderGraphNode>&& renderGraphNode);

        void compile();

        void execute(RenderPassExecuteContext& renderPassExecuteContext);

 


    };







}