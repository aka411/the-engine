#pragma once
#include <cstdint>
#include <assert.h>

namespace TheEngine::RenderingSystem
{

    //might consider CRTP to impose strict type
    constexpr uint64_t INVALID_RESOURCE_ID = UINT64_MAX;

    struct ResourceHandle 
    {
        uint64_t id = INVALID_RESOURCE_ID;

        bool isValid() const { return id != INVALID_RESOURCE_ID; }
        bool operator==(const ResourceHandle& other) const { return id == other.id; }
    };





    struct BufferHandle : public ResourceHandle {};
    struct TextureHandle : public ResourceHandle {};
    struct ShaderHandle : public ResourceHandle {};
    struct SamplerHandle : public ResourceHandle {};
    struct PipelineHandle : public ResourceHandle {};




    struct DescriptorSetLayoutHandle : public ResourceHandle {};//need more thought here


}