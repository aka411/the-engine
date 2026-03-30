

namespace TheEngine::RenderingSystem
{


	RenderingSystem::RenderingSystem(IRenderDevice&& renderDevice) :

		m_renderDevice(std::move(renderDevice)),
		m_gpuResourceSystem(m_renderDevice),

	{




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

	

		//Get icommand buffer
		m_




	}




}