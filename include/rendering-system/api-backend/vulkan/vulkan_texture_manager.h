#pragma once

namespace TheEngine::RenderingSystem::VulkanBackend
{

	class VulkanTextureManager : public ITextureManager
	{
	private:

	public:



		VulkanTextureManager();

		virtual ~VulkanTextureManager() override;

		virtual TextureHandle createNewTexture(const TextureCreateInfo& textureCreateInfo) override;

		virtual void destroyTexture(const TextureHandle& textureHandle) override;

	};






}