#pragma once
#include <vector>
#include "../../igpu_framebuffer.h"

//cleaned
namespace TheEngine::Graphics
{
	class OpenGLFramebuffer : public IGPUFramebuffer
	{
	private:
		const FramebufferCreateInfo m_openGLCreateInfo;//can only decide during use if we need to store it here
		const std::vector< GLuint> m_fbo; // index implicitively maps to each subpass

	public:
		OpenGLFramebuffer(const FramebufferCreateInfo& createInfo, std::vector< GLuint>& fbo);



		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;

		virtual uint32_t GetLayers() const override;





		virtual const std::vector<FramebufferAttachment>& GetAttachments() const override;

	};

}