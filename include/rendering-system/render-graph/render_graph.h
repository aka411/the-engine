#pragma once
#include <vector>
#include <string>
#include <memory>



#include <rendering-system/render-graph/render_graph_types.h>
#include <rendering-system/render-graph/render_graph_builder.h>
#include  <rendering-system/render-graph/i_render_graph_node.h>



namespace TheEngine::RenderingSystem
{


    class IRenderDevice;

    struct RenderPassSetupContext;
    struct RenderPassExecuteContext;


	class RenderGraph
	{


	private:

    
		std::vector<std::unique_ptr<IRenderGraphNode>> m_renderGraphNodes;

        RenderGraphBuilder m_renderGraphBuilder;

        RenderPassSetupContext m_renderPassSetupContext;
      
        WindowExtent m_currenWindowExtend;


	public:

		RenderGraph(IRenderDevice& renderDevice, const RenderPassSetupContext& renderPassSetupContext);



        void addPass(std::unique_ptr<IRenderGraphNode>&& renderGraphNode);



        //These methods below should be hidden
        void compile();

        void execute(RenderPassExecuteContext& renderPassExecuteContext);

        void onWindowResize(const WindowExtent& windowExtend);




    };







}