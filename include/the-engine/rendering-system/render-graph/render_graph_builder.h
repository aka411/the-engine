#pragma once
#include <string>


#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>
#include <rendering-system/render-graph/render_graph_types.h>


namespace TheEngine::RenderingSystem
{


    class IRenderDevice;


    class RenderGraphBuilder
    {

    private:

        IRenderDevice& m_renderDevice;

        RenderGraphResources& m_renderGraphResources;

    public:

        RenderGraphBuilder(IRenderDevice& renderDevice, RenderGraphResources& renderGraphResources);



        void createTexture(const std::string& name, TextureCreateInfo& textureCreateInfo, const ResizeParameters& resizeParameters);

        VirtualTextureId readTexture(const std::string& name);
        VirtualTextureId writeTexture(const std::string& name);

       // VirtualTextureId writeSwapChainImage();



    };


}