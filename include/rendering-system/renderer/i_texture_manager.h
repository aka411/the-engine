#pragma once
#include <vector>


namespace TheEngine::RenderingSystem
{



	class ITextureManager
	{

	private:

		// Every backend (Vulkan/GL) needs to know the metadata
		std::vector<TextureMetadata> m_textureRegistry;

		// A way to track "Free" indices for handles
		std::vector<uint32_t> m_freeIndices;

	public:

		ITextureManager();
		~ITextureManager() = default;



		//Use one method for both
		virtual TextureHandle createNewTexture(const TextureCreateInfo& textureCreateInfo) = 0;
		//virtual TextureHandle createRenderTarget(const RenderTargetCreateInfo& renderTargetCreateInfo) = 0;
		TextureMetadata getTextureMetadata(const TextureHandle& textureHandle)
		{
			assert(m_textureRegistry.size() < textureHandle.id && "ITextureManager : Invalid handle");

			return m_textureRegistry[textureHandle.id];

		}

		virtual void destroyTexture(const TextureHandle& textureHandle) = 0;

	};


}