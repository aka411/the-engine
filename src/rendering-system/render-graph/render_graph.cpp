#include <rendering-system/render-graph/render_graph.h>
#include <rendering-system/rhi/i_command_buffer.h>


namespace TheEngine::RenderingSystem
{




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


	void RenderGraph::onWindowResize(const WindowExtent& windowExtend)
	{
		m_currenWindowExtend = windowExtend;
		m_renderPassSetupContext.windowExtent = windowExtend;

		//assert(m_textureResources.size() == m_textureResourcesMapping.size());

		//delete old 
		//create new based  on new sizes 

	}


}