#include <rendering-system/rendering_system.h>
#include <rendering-system/rendering_system_data_types.h>
#include <rendering-system/rhi/i_render_device.h>
#include <rendering-system/gpu-resource-system/gpu_resource_system.h>


#include <rendering-system/rhi/i_presentation_system.h>
#include <rendering-system/rhi/i_transfer_manager.h>
#include <rendering-system/rhi/i_command_buffer.h>
#include <rendering-system/rhi/i_pipeline_manager.h>
#include <rendering-system/rhi/framebuffer.h>

#include <utils/event-bus/event_bus.h>

namespace TheEngine::RenderingSystem
{


	WindowExtent RenderingSystem::getExtent() const
	{ 
		return m_windowExtent; 
	}





	void RenderingSystem::registerOnEventBus(EventBus& bus)
	{

		m_eventBusConnection = bus.subscribe(EngineEventType::WINDOW_RESIZE, [this](const EngineEvent& event)
			{
				this->resizeWindow(event.windowResizeEvent.extend);
			});


	}



	void RenderingSystem::resizeWindow(const WindowExtent& extent)
	{
		m_presentationSystem.resizeWindow(extent);
		m_windowExtent = extent;
		m_renderGraph.onWindowResize(extent);
	}


	RenderingSystem::RenderingSystem(std::unique_ptr<IRenderDevice>&& renderDevice, TheEngine::Platform::FileSystem& filesystem,const WindowExtent& windowExtent) :

		m_renderDevice(std::move(renderDevice)),
		m_shaderSystem(filesystem, m_renderDevice->getShaderManager()),
		m_pipelineSystem(*m_renderDevice,filesystem, m_shaderSystem),
	    m_presentationSystem(m_renderDevice->getPresentationSystem()),
		m_gpuResourceSystem( std::make_unique<GPUResourceSystem>(*m_renderDevice)),
		m_renderGraph(*m_renderDevice, RenderPassSetupContext{ .pipelineSystem = m_pipelineSystem ,.shaderSystem = m_shaderSystem,.windowExtent = windowExtent },*m_gpuResourceSystem)
	{

		



	}




	RenderingSystem::~RenderingSystem()
	{


	}




	GPUResourceSystem& RenderingSystem::getGPUResourceSystem()
	{
		return *m_gpuResourceSystem;
	}


	void RenderingSystem::startRender(const UserPassData userPassData)
	{


		
		m_renderDevice->getTransferManager().flush();


		m_presentationSystem.startFrame();

		uint32_t frameIndex = m_presentationSystem.getCurrentFrameIndex();

		// tell the device (and its internal managers) to prepare for this frame
		//very important
		m_renderDevice->beginFrame(frameIndex);
		


		auto& commandBuffer = m_presentationSystem.getRenderCommandBuffer();

		RenderPassExecuteContext renderPassExecuteContext
		{
			.cmd = commandBuffer,
			.userPassData = userPassData,
			.windowExtend = m_windowExtent
		};







		m_renderGraph.execute(renderPassExecuteContext);




		//Final step
		m_presentationSystem.submitRenderCommandBuffer(commandBuffer);


		m_presentationSystem.endFrame();

	}


	RenderGraph& RenderingSystem::getRenderGraph()
	{
		return m_renderGraph;
	}



}