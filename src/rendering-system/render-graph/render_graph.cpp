#include <rendering-system/render-graph/render_graph.h>
#include <rendering-system/rhi/i_command_buffer.h>
#include <rendering-system/gpu-resource-system/gpu_resource_system.h>
#include <rendering-system/rhi/i_texture_manager.h>
namespace TheEngine::RenderingSystem
{

	void RenderGraph::onWindowResize(const WindowExtent& extent)
	{
		//WARNING : This method assumes the caller has made sure all work submitted using all these resources here are completed

		m_renderPassSetupContext.windowExtent = extent;

		for (const auto texturehandle : m_renderGraphResources.textureHandles)
		{
			m_textureManager.destroyTexture(texturehandle);
		}

		m_renderGraphResources.textureHandles.clear();

		for (const auto& renderGraphTextureCreateInfo : m_renderGraphResources.textureCreateInfos)
		{
			
			auto modifiedTextureCreateInfo = renderGraphTextureCreateInfo.textureCreateInfo;
			auto& desc = modifiedTextureCreateInfo.desc;
			switch (renderGraphTextureCreateInfo.resizeParameters.resizingMode)
			{
				case ResizingMode::ABSOLUTE:
				{
					//Do nothing
					break;
				}

				case ResizingMode::RELATIVE_FRAMEBUFFER:
				{
					const float multiplier = renderGraphTextureCreateInfo.resizeParameters.relativeMultiplier;
					desc.width = extent.framebuffer.width * multiplier;
					desc.height = extent.framebuffer.height * multiplier;
					break;
				}
				case ResizingMode::MATCH_FRAMEBUFFER:
				{
					desc.width = extent.framebuffer.width;
					desc.height = extent.framebuffer.height;
					break;
				}

			}

			m_renderGraphResources.textureHandles.push_back(m_textureManager.createTexture(modifiedTextureCreateInfo));


		}

	}


	RenderGraph::RenderGraph(IRenderDevice& renderDevice, const RenderPassSetupContext& renderPassSetupContext, GPUResourceSystem& gpuResourceSystem) :
		m_textureManager(gpuResourceSystem.getTextureManager()),
		m_renderGraphBuilder(renderDevice,m_renderGraphResources),
		m_renderPassSetupContext(renderPassSetupContext),
		m_gpuResourceResolver(gpuResourceSystem, m_renderGraphResources)
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