

namespace TheEngine::RenderingSystem
{


	RenderingSystem::RenderingSystem(IRenderDevice&& iRenderDevice) :

		m_iRenderDevice(std::move(iRenderDevice)),
		m_gpuResourceSystem(m_iRenderDevice),
		
		m_renderingContext(*m_lowLevelGPUSystem.get(), *m_gpuResourceSystem.get())
	{


		//OLD CODE REMOVE BELOW

		//add in run order
		//m_renderPasses.emplace_back(std::make_unique<ForwardRenderPass>(m_renderingContext));
		m_renderPasses.emplace_back(std::make_unique<GBufferRenderPass>(m_renderingContext));
		m_renderPasses.emplace_back(std::make_unique<LightRenderPass>(m_renderingContext));

	}




	RenderingSystem::~RenderingSystem()
	{


	}




	GPUResourceSystem& RenderingSystem::getGPUResourceSystem()
	{
		return *m_gpuResourceSystem.get();
	}





	void RenderingSystem::submitDrawCallBucket(DrawCallBucket&& drawCallbucket)
	{

		m_drawCallBucket = std::make_unique<DrawCallBucket>(std::move(drawCallbucket));

	}


	void RenderingSystem::startRender(Camera& camera)
	{

		m_renderingContext.projection = camera.projection;
		m_renderingContext.view = camera.view;

		


		for (auto& renderPass : m_renderPasses)
		{


			renderPass->excecute(*m_drawCallBucket.get());


		}





	}




}