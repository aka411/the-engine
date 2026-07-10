#include <rendering-system/rendering_system.h>
#include <rendering-system/rendering_system_data_types.h>
#include <rendering-system/rhi/i_render_device.h>
#include <rendering-system/gpu-resource-system/gpu_resource_system.h>


#include <rendering-system/rhi/i_presentation_system.h>
#include <rendering-system/rhi/i_transfer_manager.h>
#include <rendering-system/rhi/i_command_buffer.h>
#include <rendering-system/rhi/i_pipeline_manager.h>
#include <rendering-system/rhi/framebuffer.h>

namespace TheEngine::RenderingSystem
{




	bool RenderingSystem::hasResized() const 
	{ 
		return m_resizePending; 
	}

	WindowExtent RenderingSystem::getExtent() const
	{ 
		return m_windowExtent; 
	}


	void RenderingSystem::acknowledgeResize() 
	{ 
		m_resizePending = false;
	}


	RenderingSystem::RenderingSystem(std::unique_ptr<IRenderDevice>&& renderDevice, TheEngine::Platform::FileSystem& filesystem,const WindowExtent& windowExtent) :

		m_renderDevice(std::move(renderDevice)),
		m_shaderSystem(filesystem, m_renderDevice->getShaderManager()),
		m_pipelineSystem(*m_renderDevice,filesystem, m_shaderSystem),
	    m_presentationSystem(m_renderDevice->getPresentationSystem()),
		m_renderGraph(*m_renderDevice, RenderPassSetupContext{ .pipelineSystem = m_pipelineSystem ,.shaderSystem = m_shaderSystem,.windowExtent = windowExtent })
	{

		m_gpuResourceSystem = std::make_unique<GPUResourceSystem>(*m_renderDevice);
		



	}




	RenderingSystem::~RenderingSystem()
	{


	}




	GPUResourceSystem& RenderingSystem::getGPUResourceSystem()
	{
		return *m_gpuResourceSystem;
	}



	void RenderingSystem::submitDrawCallBucket(DrawCallBucket&& drawCallbucket)
	{

		m_drawCallBucket = std::make_unique<DrawCallBucket>(std::move(drawCallbucket));

	}




	void RenderingSystem::startRender(const Camera& camera)
	{


		
		m_renderDevice->getTransferManager().flush();

		if (m_resizePending)
		{
			m_presentationSystem.setWindowSize(m_windowExtent);
			m_renderGraph.onWindowResize(m_windowExtent);

			acknowledgeResize();
		}

		m_presentationSystem.startFrame();

		uint32_t frameIndex = m_presentationSystem.getCurrentFrameIndex();

		// tell the device (and its internal managers) to prepare for this frame
		//very important
		m_renderDevice->beginFrame(frameIndex);
		


		auto& commandBuffer = m_presentationSystem.getRenderCommandBuffer();

		RenderPassExecuteContext renderPassExecuteContext
		{
			.cmd = commandBuffer,
			.gpuResourceSystem = *m_gpuResourceSystem,
			.drawCallBucket = *m_drawCallBucket,
			.camera = camera,
			.windowExtend = m_windowExtent
		};







		m_renderGraph.execute(renderPassExecuteContext);




		//Final step
		m_presentationSystem.submitRenderCommandBuffer(commandBuffer);


		m_presentationSystem.endFrame();

	}

	void RenderingSystem::setWindowExtend(const WindowExtent& windowExtend)
	{
		
		m_resizePending = true;
		m_windowExtent = windowExtend;
	}



	RenderGraph& RenderingSystem::getRenderGraph()
	{
		return m_renderGraph;
	}



}