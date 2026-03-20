#include "rendering-system/renderer/rendering_system.h"
#include <rendering-system/low-level-gpu-systems/low_level_gpu_system.h>
#include <rendering-system/gpu-resource-system/gpu_resource_system.h>
#include <rendering-system/renderer/render-pass/i_render_pass.h>
#include <rendering-system/renderer/render-pass/forward_render_pass.h>


namespace TheEngine::RenderingSystem
{


	RenderingSystem::RenderingSystem():

		m_lowLevelGPUSystem(std::make_unique<LowLevelGPUSystem>()),
		m_gpuResourceSystem(std::make_unique<GPUResourceSystem>(*m_lowLevelGPUSystem.get())),
		
		m_renderingContext(*m_lowLevelGPUSystem.get(), *m_gpuResourceSystem.get())
	{


		//Create Render passes here

		m_renderPasses.emplace_back(std::make_unique<ForwardRenderPass>(m_renderingContext));


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