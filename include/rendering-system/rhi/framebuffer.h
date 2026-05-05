#pragma once
#include <array>
#include <rendering-system/engine_handles.h>
#include "data-structures/gpu_types.h"


namespace TheEngine::RenderingSystem
{



	struct AttachmentOps 
	{
		AttachmentLoadOp load = AttachmentLoadOp::DONT_CARE;
		AttachmentStoreOp store = AttachmentStoreOp::STORE;
	};

	struct RenderAttachmentInfo
	{
		TextureHandle textureHandle;

		AttachmentLoadOp attachmentLoadOp;
		AttachmentStoreOp attachmentStoreOp;

		union 
		{
			float color[4];
			struct 
			{
				float depth;
				uint32_t stencil;
			} depthStencil;
		} clearValue;

	};


	class Framebuffer
	{

	private:

		std::array<RenderAttachmentInfo,8> m_colourAttachments;
		int m_currentColorAttachmentIndex = 0;

		RenderAttachmentInfo m_depthAttachment;
		RenderAttachmentInfo m_stencilAttachment;

		bool m_hasDepth = false;
		bool m_hasStencil = false;
		bool m_isCombinedDS = false;

		unsigned int m_width = 0;
		unsigned int m_height = 0;

	public:

		Framebuffer(int width, int height);

		unsigned int getWidth() const;
		unsigned int getHeight() const;

		//consider using std::array<float, 3>
		void addColorAttachment(const TextureHandle textureHandle,const std::array<float,4>& colorClearValue, const AttachmentOps attachmentOps);

		void addDepthAttachment(const TextureHandle textureHandle, const float depthClearValue,const AttachmentOps attachmentOps);
		void addStencilAttachment(const TextureHandle textureHandle, const uint32_t stencilClearValue, const AttachmentOps attachmentOps);

		void setCombinedDepthStencil(const TextureHandle textureHandle,
			float depthClearValue,
			uint32_t stencilClearValue,
			const AttachmentOps depthAttachmentOps,
			const AttachmentOps stencilAttachmentOps
		);


		const std::array<RenderAttachmentInfo, 8>& getColorAttachments() const { return m_colourAttachments; }
		const int getColorAttachmentCount() const { return m_currentColorAttachmentIndex; }

		const RenderAttachmentInfo& getDepthAttachment() const { return m_depthAttachment; }
		const RenderAttachmentInfo& getStencilAttachment() const { return m_stencilAttachment; }

		bool hasDepth() const { return m_hasDepth; }
		bool hasStencil() const { return m_hasStencil; }
		bool isCombinedDepthStencil() const { return m_isCombinedDS; }



	};






}