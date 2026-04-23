#pragma once
#include <image_loader.h>
#include <rendering-system/engine_handles.h>






namespace TheEngine::RenderingSystem
{
	class ITextureManager;
}

namespace TheEngine::Platform
{
	class Path;
	class Platform;
}

namespace TheEngine::AssetSystem
{




	class TextureSystem
	{

	private :


		RenderingSystem::ITextureManager& m_textureManager;
		ImageLoader m_imageLoader;

	public :

		TextureSystem(RenderingSystem::ITextureManager&  textureManager, TheEngine::Platform::Platform& platform);

		RenderingSystem::TextureHandle createNewTexture(RenderingSystem::TextureCreateInfo& textureCreateInfo);

		//A helper method primarily created to help create default textures and materials
		RenderingSystem::TextureCreateInfo createDefaultTexture(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		//Use Path
		RenderingSystem::TextureHandle loadTexture(const TheEngine::Platform::Path& path);//allow sampler settings also?

	};



}