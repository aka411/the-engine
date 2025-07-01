#pragma once
#include "../../igpu_render_pass.h"

namespace TheEngine::Graphics
{
	class OpenGLRenderPass : public IGPURenderPass
	{
	private:
		const  RenderPassInfo& m_renderPassInfo;
	public:
		OpenGLRenderPass(const RenderPassInfo& renderPassInfo);

	};
}