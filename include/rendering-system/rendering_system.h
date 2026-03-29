#pragma once
#include <memory>
#include <vector>
#include  <rendering-system/renderer/rendering_context.h>
#include <rendering-system/renderer/rendering_system_data_types.h>


namespace TheEngine::Platform
{
	class Platform;
}



namespace TheEngine::RenderingSystem
{




	class GPUResourceSystem;
	class IRenderDevice;


	class RenderingSystem
	{
	private:


		std::unique_ptr<IRenderDevice> m_iRenderDevice;
		std::unique_ptr<GPUResourceSystem>  m_gpuResourceSystem;


	


	

		std::unique_ptr <DrawCallBucket> m_drawCallBucket;

	public:


		RenderingSystem(IRenderDevice&& iRenderDevice);
		~RenderingSystem();

		GPUResourceSystem& getGPUResourceSystem();

		//TODO : might need vector of buckets
		void submitDrawCallBucket(DrawCallBucket&& drawCallbucket);
		void startRender(Camera& camera);

	};


}