#pragma once
#include <vector>
#include <rendering-system/rhi/data-structures/gpu_types.h>



namespace TheEngine::RenderingSystem
{


    struct VertexBinding // More like Buffer binding
    {
        uint32_t binding;    // The Slot ID (0, 1, 2...)
        uint32_t stride;     // Bytes between vertices in THIS buffer
        bool isInstanced;    // Per-vertex or Per-instance
    };

    struct VertexAttribute
    {
        uint32_t binding;    // Which buffer slot
        uint32_t location;   // layout(location = X)

        ResourceFormat format;
        uint32_t offset;     // Offset in bytes
    };




    class VertexLayout
    {
    private:

        std::vector<VertexAttribute> m_attributes;
        std::vector<VertexBinding> m_bindings;

    public:


        VertexLayout() = default;


        void addBinding(uint32_t binding, uint32_t stride, bool isInstanced = false);
        void addAttribute(uint32_t binding, uint32_t location, ResourceFormat format, uint32_t offset);

        // Getters 
        const std::vector<VertexAttribute>& getAttributes() const;
        const std::vector<VertexBinding>& getBindings() const;

        // Helper to clear layout if reusing the object
        void clear();


    };


}