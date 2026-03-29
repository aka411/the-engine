#pragma once

namespace TheEngine::RenderingSystem
{

    //Actual Data is api specific or use resources that are wrapped
    //WHERE IS SET INDEX? Its user given at run time
    /// actual data
    struct DescriptorBinding 
    {
        uint32_t binding;

        //Yeah this aint gonna fly
        ResourceType type;
        union 
        {
            TextureHandle textureHandle;
            BufferHandle bufferHandle;//SSBO , UBO
        };

        uint32_t offset = 0; // For UBO/SSBO range
        uint32_t size = 0;   // For UBO/SSBO range

    };



    class DescriptorSetWrite
    {

    private:

        std::vector<DescriptorBinding> m_bindings;
        //std::map<uint32_t, DescriptorBinding> m_bindingPointToDescriptorBindings; //This makes more sense

    public:

        virtual ~IDescriptorSet() = default;

        //TODO : need redesigns
        // 
        // "fill" the set with actual handles
        void updateTexture(uint32_t binding, TextureHandle tex) 
        {
            // Find or add binding, then set its handle
        }

        void updateBuffer(uint32_t binding, BufferHandle buf, uint32_t size) 
        {
            // Find or add binding, then set its handle
        }

        const std::vector<DescriptorBinding>& getBindings() const 
        { 
            return m_bindings; 
        }

    };


}