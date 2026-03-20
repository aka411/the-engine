#pragma once
#include <memory>
#include <vector>
#include  <rendering-system/renderer/rendering_context.h>
#include <rendering-system/renderer/rendering_system_data_types.h>

namespace TheEngine::RenderingSystem
{




	class GPUResourceSystem;
	class LowLevelGPUSystem;

	class IRenderPass;

	//class RenderingContext;


	class RenderingSystem
	{
	private:


		std::unique_ptr<LowLevelGPUSystem> m_lowLevelGPUSystem;
		std::unique_ptr<GPUResourceSystem>  m_gpuResourceSystem;
	


		RenderingContext m_renderingContext;

		std::vector<std::unique_ptr<IRenderPass>> m_renderPasses;

		std::unique_ptr <DrawCallBucket> m_drawCallBucket;
	public:

		RenderingSystem();
		~RenderingSystem();

		GPUResourceSystem& getGPUResourceSystem();

		//TODO : might need vector of buckets
		void submitDrawCallBucket(DrawCallBucket&& drawCallbucket);
		void startRender(Camera& camera);

	};


}