#pragma once
#include <string>
#include <unordered_map>
#include <rendering-system/engine_handles.h>
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>


namespace TheEngine::RenderingSystem
{
    class IRenderDevice;

    class RenderGraphBuilder
    {

    private:

        IRenderDevice& m_renderDevice;

        //ResourceRegistry& m_resourceRegistry;
        std::unordered_map<std::string, TextureHandle> m_textureResources;


        //FRAME DATA



    public:

        RenderGraphBuilder(IRenderDevice& renderDevice);



        TextureHandle createTexture(const std::string& name, TextureCreateInfo& textureCreateInfo);

        TextureHandle readTexture(const std::string& name);//string based look up
        TextureHandle writeTexture(const std::string& name);


        void deleteTexture(const TextureHandle textureHandle);

    };







}