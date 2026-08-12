#include <rendering-system/render-graph/render_graph_builder.h>
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>
#include <rendering-system/rhi/i_render_device.h>

#include <rendering-system/rhi/i_texture_manager.h>
#include <rendering-system/render-graph/render_graph.h>

namespace TheEngine::RenderingSystem
{


    RenderGraphBuilder::RenderGraphBuilder(IRenderDevice& renderDevice, RenderGraphResources& renderGraphResources):
        m_renderDevice(renderDevice),
        m_renderGraphResources(renderGraphResources)
    {

    }



    void RenderGraphBuilder::createTexture(const std::string& name, TextureCreateInfo& textureCreateInfo, const ResizeParameters& resizeParameters)
    {
        VirtualTextureId vId;
        vId.id = m_renderGraphResources.textureCreateInfos.size();

        RenderGraphTextureCreateInfo renderGraphTextureCreateInfo;
        renderGraphTextureCreateInfo.resizeParameters = resizeParameters;
        renderGraphTextureCreateInfo.textureCreateInfo = textureCreateInfo;

        m_renderGraphResources.textureCreateInfos.push_back(renderGraphTextureCreateInfo);

        TextureHandle textureHandle = m_renderDevice.getTextureManager().createTexture(textureCreateInfo);
        m_renderGraphResources.textureHandles.push_back(textureHandle);

        m_renderGraphResources.textureResources.insert({ name,vId });

    }


    VirtualTextureId RenderGraphBuilder::readTexture(const std::string& name)
    {
        return m_renderGraphResources.textureResources.at(name);
    }


    VirtualTextureId RenderGraphBuilder::writeTexture(const std::string& name)
    {
        return  m_renderGraphResources.textureResources.at(name);
    }


    /*
    void RenderGraphBuilder::writeSwapChainImage()
    {
        return;
    }
    */
}