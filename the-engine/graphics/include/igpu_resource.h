#pragma once

//cleaned
namespace TheEngine::Graphics
{

    enum class ResourceType
    {
        BUFFER,
        TEXTURE,
        SHADER,
        PIPELINE,
        FRAMEBUFFER,
        SAMPLER,
        COMMANDLIST,

        MESH,
        MATERIAL,
        // Add more types as we need
        UNKNOWN
    };


    
    class IGPUResource
    {
    protected:
		IGPUResource() = default; // Protected constructor to prevent direct instantiation

    public:



        virtual ~IGPUResource() = default;

        // Get the type of this GPU resource
        virtual ResourceType getType() const = 0;//consider if we need it



    };
}
