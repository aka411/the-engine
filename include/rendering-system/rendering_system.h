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



		WindowExtent m_windowExtent;
		bool m_resizePending = false;

		bool hasResized() const;
		void acknowledgeResize();

	public:


		RenderingSystem(std::unique_ptr<IRenderDevice>&& renderDevice, TheEngine::Platform::FileSystem& filesystem, const WindowExtent& windowExtent);
		~RenderingSystem();

		GPUResourceSystem& getGPUResourceSystem();


		//TODO : might need vector of buckets
		void submitDrawCallBucket(DrawCallBucket&& drawCallbucket);
		void startRender(const Camera& camera);

		
		void setWindowExtend(const WindowExtent& windowExtend);
		WindowExtent getExtent() const;


		RenderGraph& getRenderGraph();
		//void addPass(const std::string& name, SetupFunc setupFunc);


	};


}