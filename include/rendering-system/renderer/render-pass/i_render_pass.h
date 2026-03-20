#pragma once
#include <rendering-system/renderer/rendering_system_data_types.h>


namespace TheEngine::RenderingSystem
{

	class LowLevelGPUSystem;
	class GPUResourceSystem;
	class RenderingContext;

	//struct DrawCallBucket;

	class IRenderPass
	{

	protected:

		LowLevelGPUSystem& m_lowLevelGPUSystem;
		GPUResourceSystem& m_gpuResourceSystem;


		RenderingContext& m_renderingContext;

	public:

		IRenderPass(RenderingContext& renderingContext);



		//virtual void onScreenResize();

		virtual void excecute(const DrawCallBucket& drawCallbucket) = 0;

	};


}