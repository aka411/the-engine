

namespace TheEngine::RenderingSystem::OpenGLBackend
{


	GPUPipelineManager::GPUPipelineManager()

	{




	}



    GPUPipelineManager::~GPUPipelineManager()
    {


    }



    PipelineHandle GPUPipelineManager::createPipeline(PipelineDescriptor& pipelineDescriptor)
    {

        PipelineHandle handle = m_gpuPipelines.size();

        m_gpuPipelines.emplace_back(std::make_unique<TheEngine::RenderingSystem::OpenGLBackend::OpenglPipelineStateObject>(pipelineDescriptor));

        return handle;

    }


    IGPUPipeline& GPUPipelineManager::getGPUPipelineState(const PipelineHandle pipelineHandle)
    {
        assert(m_gpuPipelines.size() > pipelineHandle);

        return *m_gpuPipelines[pipelineHandle];
    }

}