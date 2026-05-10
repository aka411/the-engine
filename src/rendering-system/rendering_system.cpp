#include <rendering-system/rendering_system.h>
#include <rendering-system/rendering_system_data_types.h>
#include <rendering-system/rhi/i_render_device.h>
#include <rendering-system/gpu-resource-system/gpu_resource_system.h>


#include <rendering-system/rhi/i_pipeline_builder.h>
#include <rendering-system/rhi/i_transfer_manager.h>
#include <rendering-system/rhi/i_presentation_system.h>

#include <rendering-system/rhi/i_command_buffer.h>
#include <rendering-system/rhi/i_pipeline_manager.h>
#include <rendering-system/rhi/framebuffer.h>

namespace TheEngine::RenderingSystem
{


	RenderingSystem::RenderingSystem(std::unique_ptr<IRenderDevice>&& renderDevice, TheEngine::Platform::FileSystem& filesystem) :

		m_renderDevice(std::move(renderDevice)),
		m_shaderSystem(filesystem, m_renderDevice->getShaderManager()),
		m_pipelineSystem(*m_renderDevice,filesystem, m_shaderSystem),
	    m_presentationSystem(m_renderDevice->getPresentationSystem()),
		m_renderGraph(*m_renderDevice, RenderPassSetupContext{ .pipelineSystem = m_pipelineSystem ,.shaderSystem = m_shaderSystem })
	{

		m_gpuResourceSystem = std::make_unique<GPUResourceSystem>(*m_renderDevice);
		


/*
		ShaderCreateInfoFromFile shaderCreateInfoFromFile;

		shaderCreateInfoFromFile.shaderSourceFilePaths.insert({ ShaderType::VERTEX, TheEngine::Platform::Path(TheEngine::Platform::MountPoint::ENGINE,"shaders/vulkan/forward_vert.glsl") });
		shaderCreateInfoFromFile.shaderSourceFilePaths.insert({ ShaderType::FRAGMENT, TheEngine::Platform::Path(TheEngine::Platform::MountPoint::ENGINE, "shaders/vulkan/forward_frag.glsl") });

		ShaderHandle shaderHandle = m_shaderSystem.createShaderFromFile(shaderCreateInfoFromFile);


		PipelineStateConfig pipelineStateConfig = m_pipelineSystem
			.loadPipelineConfigFromFile(
				TheEngine::Platform::Path(TheEngine::Platform::MountPoint::ENGINE,
					"pipeline-state-config/pipeline_config.json"));

		RenderOutputConfiguration renderOutputConfiguration = m_pipelineSystem
			.loadRenderOutputConfigurationFromFile(
				TheEngine::Platform::Path(TheEngine::Platform::MountPoint::ENGINE,
					"render-output-config/render_config.json"));
		


		std::unique_ptr<IPipelineBuilder> pipelineBuilder =  m_renderDevice->getPipelineBuilder();
		pipelineBuilder->setName("Default")
			.setShader(shaderHandle)
			.setPipelineConfig(pipelineStateConfig)
			//.setPipelineLayout() // Use reflection for now, this is a headache casue 
			.setRenderOutputConfiguration(renderOutputConfiguration);
			


			// 
		//Build fails
		 PipelineHandle pipelineHandle = pipelineBuilder->build();
		 



		 */




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
			.camera = camera
		};







		m_renderGraph.execute(renderPassExecuteContext);



		//Final step
		m_presentationSystem.submitRenderCommandBuffer(commandBuffer);




	}


	RenderGraph& RenderingSystem::getRenderGraph()
	{
		return m_renderGraph;
	}



}