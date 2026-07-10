#include <rendering-system/rhi/texture_builder.h>



namespace TheEngine::RenderingSystem
{


    
        TextureBuilder& TextureBuilder::setDimensions(uint32_t width, uint32_t height, uint32_t depth) 
        {
            m_Info.desc.width = width;
            m_Info.desc.height = height;
            m_Info.desc.depth = depth;
            return *this;
        }

        TextureBuilder& TextureBuilder::setType(TextureType type)
        {
            m_Info.desc.type = type;
            return *this;
        }

        TextureBuilder& TextureBuilder::setFormat(ResourceFormat format) {
            m_Info.desc.format = format;
            return *this;
        }

 
        TextureBuilder& TextureBuilder::setMipLevels(uint32_t levels)
        {
            m_Info.desc.mipLevelCount = levels;
            return *this;
        }

        TextureBuilder& TextureBuilder::setLayers(uint32_t layers) 
        {
            m_Info.desc.arrayLayerCount = layers;
            return *this;
        }

     
        TextureBuilder& TextureBuilder::setUsage(TextureUsageFlags flags) 
        {
            m_Info.desc.usageFlags = flags;
            return *this;
        }

        TextureBuilder& TextureBuilder::addUsage(TextureUsageFlags flag) 
        {
            m_Info.desc.usageFlags |= flag;
            return *this;
        }

        TextureBuilder& TextureBuilder::setSamples(SampleCountBit sampleCountBit)
        {
            m_Info.desc.sampleCount = sampleCountBit;
            return *this;
        }

     
        TextureBuilder& TextureBuilder::setMemoryBlock(Memory::MemoryBlock&& block)
        {
            m_Info.memoryBlock = std::move(block);
            return *this;
        }

        TextureBuilder& TextureBuilder::setInitialLayout(TextureLayout layout)
        {
            m_Info.initialLayout = layout;
            return *this;
        }

 
        TextureBuilder& TextureBuilder::setSamplerInfo(const SamplerCreateInfo& samplerInfo) 
        {
            m_Info.samplerCreateInfo = samplerInfo;
            m_Info.useSampler = true;
            return *this;
        }

        TextureBuilder& TextureBuilder::setBindless(bool enable) 
        {
            m_Info.makeBindless = enable;
            return *this;
        }





        [[nodiscard]] TextureCreateInfo TextureBuilder::build()
        {
            return std::move(m_Info);
        }


}