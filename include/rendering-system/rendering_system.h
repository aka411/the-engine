#pragma once
#include <memory>
#include <vector>
#include <rendering-system/rendering_system_data_types.h>
#include <rendering-system/shader_system.h>
#include <rendering-system/pipeline_system.h>
#include <rendering-system/render-graph/render_graph.h>



namespace TheEngine::Platform
{
	class Platform;
}




namespace TheEngine::RenderingSystem
{

	class IPresentationSystem;


	class GPUResourceSystem;
	class IRenderDevice;


	class RenderingSystem
	{
	private:


		std::unique_ptr<IRenderDevice> m_renderDevice;

		std::unique_ptr<GPUResourceSystem>  m_gpuResourceSystem;

		IPresentationSystem& m_presentationSystem;


		ShaderSystem m_shaderSystem;
	
		PipelineSystem m_pipelineSystem;

		std::unique_ptr <DrawCallBucket> m_drawCallBucket;

		RenderGraph m_renderGraph;


	public:


		RenderingSystem(std::unique_ptr<IRenderDevice>&& renderDevice, TheEngine::Platform::FileSystem& filesystem);
		~RenderingSystem();

		GPUResourceSystem& getGPUResourceSystem();


		//TODO : might need vector of buckets
		void submitDrawCallBucket(DrawCallBucket&& drawCallbucket);
		void startRender(const Camera& camera);



		RenderGraph& getRenderGraph();



	};


}