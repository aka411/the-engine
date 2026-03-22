#include "asset-system/texture_system.h"
#include "rendering-system/low-level-gpu-systems/gpu_texture_manager.h"
#include <platform/platform.h>


namespace TheEngine::AssetSystem
{


	TextureSystem::TextureSystem(RenderingSystem::GPUTextureManager& gpuTextureManager, TheEngine::Platform::Platform& platform) :
		m_gpuTextureManager(gpuTextureManager),
		m_imageLoader(platform.getFileSystem())
	{



	}


	TheEngine::RenderingSystem::TextureInfo TextureSystem::createNewTexture(const TheEngine::RenderingSystem::TextureCreateInfo& textureCreateInfo)
	{

		return m_gpuTextureManager.createNewTexture(textureCreateInfo);
	}



    //Helper function
    RenderingSystem::TextureCreateInfo TextureSystem::createDefaultTexture(uint8_t r, uint8_t g, uint8_t b, uint8_t a) 
    {
      
        using namespace RenderingSystem;

        RenderingSystem::TextureCreateInfo info;
        info.width = 1;
        info.height = 1;
        info.type = TextureType::TEXTURE_2D;
        info.internalFormat = TextureInternalFormat::RGBA8;
        info.textureSourcePixelFormat = TextureSourcePixelFormat::RGBA;
        info.textureSourceComponentType = TextureSourceComponentType::UNSIGNED_BYTE;


        /*
        info.samplerSetting.minFilter = TextureFilter::NEAREST;
        info.samplerSetting.magFilter = TextureFilter::NEAREST;
        info.samplerSetting.wrapS = TextureWrap::REPEAT;
        info.samplerSetting.wrapT = TextureWrap::REPEAT;
        */

         uint8_t pixels[4] =  { r, g, b, a };

        info.memoryBlock = TheEngine::Memory::MemoryBlock(
            reinterpret_cast<std::byte*>(pixels),
            4
        );

        return info;
    }




	RenderingSystem::TextureInfo TextureSystem::loadTexture(const TheEngine::Platform::Path& path)
	{
		return m_gpuTextureManager.createNewTexture(m_imageLoader.loadTextureFile(path));
	}



}