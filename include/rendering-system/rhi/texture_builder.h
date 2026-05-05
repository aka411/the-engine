#pragma once
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>

namespace TheEngine::RenderingSystem
{
    class TextureBuilder
    {

    private:

        TextureCreateInfo m_Info{};



    public:

        TextureBuilder() = default;

    
        TextureBuilder& setDimensions(uint32_t width, uint32_t height, uint32_t depth = 1);

        TextureBuilder& setType(TextureType type);
      

        TextureBuilder& setFormat(ResourceFormat format);

       
        TextureBuilder& setMipLevels(uint32_t levels);

        TextureBuilder& setLayers(uint32_t layers);

        // Overwrite
        TextureBuilder& setUsage(TextureUsageFlags flags);

        //Logical OR - ing
        TextureBuilder& addUsage(TextureUsageFlags flag);

        TextureBuilder& setSamples(uint32_t count);

        // Sinks data in
        TextureBuilder& setMemoryBlock(Memory::MemoryBlock&& block);

        TextureBuilder& setInitialLayout(TextureLayout layout);

    
        TextureBuilder& setSamplerInfo(const SamplerCreateInfo& samplerInfo);

        TextureBuilder& setBindless(bool enable);


        [[nodiscard]] TextureCreateInfo build();


    };
}