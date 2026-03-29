#pragma once

namespace TheEngine::RenderingSystem
{

    //might consider CRTP to impose strict type

    struct ResourceHandle {
        uint32_t id;

        bool is_valid() const { return id != 0; }
        bool operator==(const ResourceHandle& other) const { return id == other.id; }
    };





    struct BufferHandle : public ResourceHandle {};
    struct TextureHandle : public ResourceHandle {};
    struct ShaderHandle : public ResourceHandle {};
    struct SamplerHandle : public ResourceHandle {};
    struct PipelineHandle : public ResourceHandle {};

    struct DescriptorSetLayoutHandle : public ResourceHandle {};//need more thought here
    //FBO handle??

}