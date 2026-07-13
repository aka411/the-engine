#include <rendering-system/api-backend/opengl/opengl_pipeline_manager.h>

namespace TheEngine::RenderingSystem::OpenGLBackend
{


    OpenglPipelineManager::OpenglPipelineManager()

	{




	}



    OpenglPipelineManager::~OpenglPipelineManager()
    {


    }




    const PipelineHandle OpenglPipelineManager::createPipelineStateObject(const PipelineStateConfiguration& pipelineStateConfiguration)
    {

        PipelineHandle handle;
        handle.id = m_openglPipelineCache.size();

        m_openglPipelineCache.push_back(pipelineStateConfiguration);

        return handle;

    }





}