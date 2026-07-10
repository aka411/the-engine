#include <rendering-system/render-graph/render_graph_builder.h>
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>
#include <rendering-system/rhi/i_render_device.h>

#include <rendering-system/rhi/i_texture_manager.h>

namespace TheEngine::RenderingSystem
{


    RenderGraphBuilder::RenderGraphBuilder(IRenderDevice& renderDevice):
        m_renderDevice(renderDevice)
    {

    }



    TextureHandle RenderGraphBuilder::createTexture(const std::string& name, TextureCreateInfo& textureCreateInfo)
    {
        TextureHandle textureHandle = m_renderDevice.getTextureManager().createNewTexture(textureCreateInfo);

        m_textureResources.insert({ name,textureHandle });

        return textureHandle;
    }


    TextureHandle RenderGraphBuilder::readTexture(const std::string& name)
    {


        return  m_textureResources.at(name);
    }


    TextureHandle RenderGraphBuilder::writeTexture(const std::string& name)
    {

        return  m_textureResources.at(name);
    }


    void deleteTexture(const TextureHandle textureHandle)
    {

    }


}