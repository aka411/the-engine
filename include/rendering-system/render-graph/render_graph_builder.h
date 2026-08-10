#pragma once
#include <string>
#include <unordered_map>
#include <rendering-system/engine_handles.h>
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>
#include <map>


namespace TheEngine::RenderingSystem
{


    class IRenderDevice;


    class RenderGraphBuilder
    {

    private:

        IRenderDevice& m_renderDevice;

        std::unordered_map<std::string, TextureHandle> m_textureResources;
       // std::unordered_map<TextureHandle, std::string> m_textureResourcesMapping;
        // std::map<uint64_t, TextureHandle> m_textureBufferResource;

         //FRAME DATA



    public:

        RenderGraphBuilder(IRenderDevice& renderDevice);



        TextureHandle createTexture(const std::string& name, TextureCreateInfo& textureCreateInfo);

        TextureHandle readTexture(const std::string& name);//string based look up
        TextureHandle writeTexture(const std::string& name);

        TextureHandle writeSwapChainImage();



    };


}