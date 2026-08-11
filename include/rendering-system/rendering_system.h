#pragma once
#include <memory>
#include <vector>
#include <rendering-system/rendering_system_data_types.h>
#include <rendering-system/shader_system.h>
#include <rendering-system/pipeline_system.h>
#include <rendering-system/render-graph/render_graph.h>
#include <utils/event-bus/event_bus_connection.h>



namespace TheEngine::Platform
{
	class Platform;
}


namespace TheEngine
{
	class EventBus;
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

	private:
		ShaderSystem m_shaderSystem;
	
		PipelineSystem m_pipelineSystem;


	private:

		RenderGraph m_renderGraph;



		WindowExtent m_windowExtent;

		EventBusConnection m_eventBusConnection;

	private:

		void resizeWindow(const WindowExtent& extent);

	public:


		RenderingSystem(std::unique_ptr<IRenderDevice>&& renderDevice, TheEngine::Platform::FileSystem& filesystem, const WindowExtent& windowExtent);
		~RenderingSystem();

		void registerOnEventBus(EventBus& bus);

		GPUResourceSystem& getGPUResourceSystem();



		void startRender(const UserPassData userPassData);

		
		WindowExtent getExtent() const;


		RenderGraph& getRenderGraph();
	


	};


}