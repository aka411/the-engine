#include "asset-system/texture_system.h"
#include <rendering-system/rhi/i_texture_manager.h>
#include <platform/platform.h>


namespace TheEngine::AssetSystem
{


	TextureSystem::TextureSystem(RenderingSystem::ITextureManager& textureManager, TheEngine::Platform::Platform& platform) :
		m_textureManager(textureManager),
		m_imageLoader(platform.getFileSystem())
	{



	}


    RenderingSystem::TextureHandle TextureSystem::createNewTexture(const TheEngine::RenderingSystem::TextureCreateInfo& textureCreateInfo)
	{

		return m_textureManager.createTexture(textureCreateInfo);
	}



    //Helper function
    RenderingSystem::TextureHandle TextureSystem::createDefaultTexture(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
      
        //TODO : replace with builder pattern
        using namespace RenderingSystem;

        RenderingSystem::TextureCreateInfo textureCreateInfo;

        textureCreateInfo.desc.width = 1;
        textureCreateInfo.desc.height = 1;
        textureCreateInfo.desc.type = TextureType::TEXTURE_2D;
        textureCreateInfo.desc.format = ResourceFormat::RGBA8_UNORM;
    
     

        /*
        info.samplerSetting.minFilter = TextureFilter::NEAREST;
        info.samplerSetting.magFilter = TextureFilter::NEAREST;
        info.samplerSetting.wrapS = TextureWrap::REPEAT;
        info.samplerSetting.wrapT = TextureWrap::REPEAT;
        */

         uint8_t pixels[4] =  { r, g, b, a };

         Memory::MemoryBlock memoryBlock = TheEngine::Memory::MemoryBlock(
            reinterpret_cast<std::byte*>(pixels),
            4
        );
         return m_textureManager.createTexture(textureCreateInfo, std::move(memoryBlock));
    }




    RenderingSystem::TextureHandle TextureSystem::loadTexture(const TheEngine::Platform::Path& path)
	{
        Memory::MemoryBlock memoryBlock;
        const TheEngine::RenderingSystem::TextureCreateInfo textureCreateInfo = m_imageLoader.loadTextureFile(path, memoryBlock);
		return m_textureManager.createTexture(textureCreateInfo, std::move(memoryBlock));
	}



}