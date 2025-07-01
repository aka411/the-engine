#pragma once

// ok cleaned for now
namespace TheEngine::Graphics
{



    enum class SetIndex : uint32_t
    {
        // Global data that changes infrequently or per-frame for the whole scene
        GLOBAL_DATA_SET = 0,

        // Data that changes per object/draw call (e.g., model matrix, instance data)
        OBJECT_DATA_SET = 1,

        // Data specific to a material (e.g., PBR factors, albedo map, normal map)
        MATERIAL_DATA_SET = 2,

        // Additional sets for things like skeletal animation, particle systems, etc.
        ANIMATION_DATA_SET = 3,

        COUNT // Keep track of the total number of sets
    };


    // Enums for resource types and shader stages (API-agnostic)
    enum class DescriptorType
    {
        
        SAMPLER,
        COMBINED_IMAGE_SAMPLER, // Texture + Sampler
        SAMPLED_IMAGE,
        STORAGE_IMAGE,
        UNIFORM_BUFFER,
        STORAGE_BUFFER,

        UNIFORM_BUFFER_DYNAMIC, // For one or multiple monolith with per object model data , 
                                // we provide offsets at draw time can reduce number 
                                // of bind calls to one instead of n for n objects
    
        STORAGE_BUFFER_DYNAMIC,
        // Add more types as needed (e.g., InputAttachment for subpasses)
    };

    enum class ShaderStageFlags
    {
        NONE = 0,
        VERTEX = 1 << 0,
       // TessellationControl = 1 << 1,
       // TessellationEvaluation = 1 << 2,
        GEOMETRY = 1 << 3,
        FRAGMENT = 1 << 4,
      //  Compute = 1 << 5,
      //  AllGraphics = Vertex | TessellationControl | TessellationEvaluation | Geometry | Fragment,
      //  All = AllGraphics | Compute
    };

    // Structure to define a single binding within the layout
    //send this to the GPU to create a  IGPUDescriptorSet
    //for multiple bindings, you would create a vector of these
    struct DescriptorSetLayoutBinding // Represents a single binding in a descriptor set layout
    {
        uint32_t binding;           // Corresponds to 'layout(binding = X)' in shader
        DescriptorType descriptorType; // Type of the resource (e.g., UniformBuffer, SampledImage)
        uint32_t descriptorCount;   // Number of descriptors if it's an array (usually 1)
        ShaderStageFlags stageFlags; // Which shader stages can access this binding
        // bool immutableSampler; // Optional: for immutable samplers in Vulkan
    };


    //created by the IGPURenderDevice by passing a vector of DescriptorSetLayoutBinding
    //will be used to create IGPUPipelineLayout.
    //This is per set object
	// IGPUPipelineLayout will have a vector of IGPUDescriptorSetLayout with index of vector as set number
    //vulkan implementation will have a VkDescriptorSetLayout as member and a getDescriptorSetLayout method
    class IGPUDescriptorSetLayout : public IGPUResource
    {
    protected:
        // Protected constructor to prevent direct instantiation.
        // Descriptor set layouts are created via the IGPURenderDevice.
        IGPUDescriptorSetLayout() = default;

    public:
        // Virtual destructor for proper polymorphic cleanup.
        virtual ~IGPUDescriptorSetLayout() = default;
        virtual ResourceType getType() const override { return ResourceType::UNKNOWN; };//consider removing this 
        // No public virtual methods are typically needed here, as the layout's
        // properties are fixed at creation.
    };

}