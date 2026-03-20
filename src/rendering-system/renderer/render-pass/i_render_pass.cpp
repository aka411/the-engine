#include <rendering-system/renderer/render-pass/i_render_pass.h>
#include <rendering-system/renderer/rendering_context.h>



namespace TheEngine::RenderingSystem
{



		IRenderPass::IRenderPass(RenderingContext& renderingContext):
			m_lowLevelGPUSystem(renderingContext.getLowLevelGPUSystem()),
			m_gpuResourceSystem(renderingContext.getGPUResourceSystem()),

			m_renderingContext(renderingContext)
		{



		}



}