#pragma once
#include <vector>
#include <string>




#include <rendering-system/render-graph/render_graph_types.h>
#include <rendering-system/render-graph/render_graph_builder.h>



namespace TheEngine::RenderingSystem
{


    class IRenderDevice;

    struct RenderPassSetupContext;
    struct RenderPassExecuteContext;


	class RenderGraph
	{


	private:

    
		std::vector<RenderGraphNode> m_renderGraphNodes;

        RenderGraphBuilder m_renderGraphBuilder;


        RenderPassSetupContext m_renderPassSetupContext;
      


	public:

		RenderGraph(IRenderDevice& renderDevice, const RenderPassSetupContext& renderPassSetupContext);


        void addPass(const std::string& name, SetupFunc setupFunc);

        void compile();

        void execute(RenderPassExecuteContext& renderPassExecuteContext);






    };







}