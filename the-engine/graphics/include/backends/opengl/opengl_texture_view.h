#pragma once
#include "../../igpu_texture_view.h"
namespace TheEngine::Graphics
{

	class OpenGLTextureView : public IGPUTextureView
	{
	private:
		const OpenGLTexture& m_texture;//pointer maybe?
		const  TextureViewCreateInfo m_createInfo;
	public:
		OpenGLTextureView(const TextureViewCreateInfo& createInfo, const OpenGLTexture& texture)
			: IGPUTextureView(createInfo), m_texture(texture)
		{
		}
		// Get the texture this view refers to (not owned)
		virtual IGPUTexture* GetTexture() const override { return const_cast<OpenGLTexture*>(&m_texture); }
		// Getters for view properties
		virtual TextureViewType GetViewType() const override { return m_viewType; }
		virtual TextureFormat GetFormat() const override { return m_format; }
		virtual TextureAspectFlags GetAspectMask() const override { return m_aspectMask; }
		virtual uint32_t GetBaseMipLevel() const override { return m_baseMipLevel; }
		virtual uint32_t GetMipLevelCount() const override { return m_mipLevelCount; }
		virtual uint32_t GetBaseArrayLayer() const override { return m_baseArrayLayer; }
		virtual uint32_t GetArrayLayerCount() const override { return m_arrayLayerCount; }

	};
}