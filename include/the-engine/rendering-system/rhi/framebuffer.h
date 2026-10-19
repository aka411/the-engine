#pragma once
#include <array>
#include <rendering-system/engine_handles.h>
#include <rendering-system/rhi/data-structures/gpu_types.h>
#include <glm/vec4.hpp>


namespace TheEngine::RenderingSystem
{



	struct AttachmentOps 
	{
		AttachmentLoadOp load = AttachmentLoadOp::DONT_CARE;
		AttachmentStoreOp store = AttachmentStoreOp::STORE;
	};

	struct RenderAttachmentInfo
	{
		// The primary render target (MSAA buffer or single-sample image)
		TextureHandle image;

		//The single-sample destination for MSAA resolve
		TextureHandle resolveImage;

		AttachmentOps imageAttachmentOps;
		AttachmentOps resolveImageAttachmentOps;


		AttachmentResolveMode resolveMode = AttachmentResolveMode::NONE;

		union
		{
			glm::vec4 colour;
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
		uint8_t m_currentColorAttachmentIndex = 0;

		RenderAttachmentInfo m_depthAttachment;
		RenderAttachmentInfo m_stencilAttachment;

		bool m_isMSAA = false;

		bool m_hasDepth = false;
		bool m_hasStencil = false;
		bool m_isCombinedDS = false;

		uint32_t m_width = 0;
		uint32_t m_height = 0;

	public:

		Framebuffer(const uint32_t width, const uint32_t height);

		unsigned int getWidth() const;
		unsigned int getHeight() const;

	
		void addColorAttachment(const uint32_t location, const TextureHandle textureHandle,const glm::vec4& colorClearValue, const AttachmentOps attachmentOps);
		void addMultisampledColorTarget(uint32_t location, TextureHandle msaaBuffer, TextureHandle finalResult, const glm::vec4& colorClearValue);
		
		





		void addDepthAttachment(const TextureHandle textureHandle, const float depthClearValue,const AttachmentOps attachmentOps);
		void addStencilAttachment(const TextureHandle textureHandle, const uint32_t stencilClearValue, const AttachmentOps attachmentOps);




		void setCombinedDepthStencil(const TextureHandle textureHandle,
			float depthClearValue,
			uint32_t stencilClearValue,
			const AttachmentOps depthAttachmentOps,
			const AttachmentOps stencilAttachmentOps
		);



		void addMultisampledDepthTargetTransient(
			const TextureHandle msaaDepthBuffer,
			float depthClearValue,
			const AttachmentOps depthOps,
			AttachmentResolveMode resolveMode);


		void addMultisampledDepthStencilTargetTransient(
			const TextureHandle msaaDepthBuffer,
			float depthClearValue,
			uint32_t stencilClearValue,
			const AttachmentOps depthOps,
			const AttachmentOps stencilOps,
			AttachmentResolveMode resolveMode);





		const std::array<RenderAttachmentInfo, 8>& getColorAttachments() const { return m_colourAttachments; }
		const int getColorAttachmentCount() const { return m_currentColorAttachmentIndex; }

		const RenderAttachmentInfo& getDepthAttachment() const { return m_depthAttachment; }
		const RenderAttachmentInfo& getStencilAttachment() const { return m_stencilAttachment; }

		bool hasDepth() const { return m_hasDepth; }
		bool hasStencil() const { return m_hasStencil; }
		bool isCombinedDepthStencil() const { return m_isCombinedDS; }

		bool enabledMSAA() const { return m_isMSAA; }

	};






}