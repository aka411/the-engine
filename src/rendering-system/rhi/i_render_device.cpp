#include <rendering-system/rhi/i_render_device.h>

#include <rendering-system/rhi/i_shader_manager.h>
#include <rendering-system/rhi/i_buffer_manager.h>
#include <rendering-system/rhi/i_texture_manager.h>
#include <rendering-system/rhi/i_sampler_manager.h>
#include <rendering-system/rhi/i_pipeline_manager.h>
#include <rendering-system/rhi/i_transfer_manager.h>
#include <rendering-system/rhi/i_presentation_system.h>

namespace TheEngine::RenderingSystem
{


	IRenderDevice::IRenderDevice(const RenderingAPI renderingAPI) :
	m_renderingAPI(renderingAPI)
	{

	}



	IRenderDevice::~IRenderDevice()
	{

	}


	IBufferManager& IRenderDevice::getBufferManager() 
	{
		return *m_bufferManager;
	}


	ISamplerManager& IRenderDevice::getSamplerManager() 
	{
		return *m_samplerManager;
	}

	ITextureManager& IRenderDevice::getTextureManager() 
	{
		return *m_textureManager;
	}

	IShaderManager& IRenderDevice::getShaderManager() 
	{
		return *m_shaderManager;
	}

	ITransferManager& IRenderDevice::getTransferManager() 
	{
		return *m_transferManager;
	}

	IPipelineManager& IRenderDevice::getPipelineManager() 
	{
		return *m_pipelineManager;
	}

	IPresentationSystem& IRenderDevice::getPresentationSystem()
	{
		return *m_presentationSystem;
	}

}