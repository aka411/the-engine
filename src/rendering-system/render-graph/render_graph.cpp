#include <rendering-system/render-graph/render_graph.h>
#include <rendering-system/rhi/i_command_buffer.h>


namespace TheEngine::RenderingSystem
{

	void RenderGraph::onWindowResize(const WindowExtent& extent)
	{
		
		m_renderPassSetupContext.windowExtent = extent;

		//assert(m_textureResources.size() == m_textureResourcesMapping.size());

		//delete old 
		//create new based  on new sizes 

	}


	RenderGraph::RenderGraph(IRenderDevice& renderDevice, const RenderPassSetupContext& renderPassSetupContext, GPUResourceSystem& gpuResourceSystem) :
		m_renderGraphBuilder(renderDevice),
		m_renderPassSetupContext(renderPassSetupContext),
		m_gpuResourceResolver(gpuResourceSystem)
	{



	}






	void RenderGraph::addPass(std::unique_ptr<IRenderGraphNode>&& renderGraphNode)
	{

		m_renderGraphNodes.push_back(std::move(renderGraphNode));
	}





	void RenderGraph::compile()
	{


		for (auto& node : m_renderGraphNodes)
		{

			node->setUp(m_renderPassSetupContext,m_renderGraphBuilder);


		}


	}




	void RenderGraph::execute(RenderPassExecuteContext& renderPassExecuteContext)
	{
		
		for (auto& node : m_renderGraphNodes)
		{
			node->execute(renderPassExecuteContext,m_gpuResourceResolver);
		}



	}

}