#pragma once


#include <cstdint> // Required for uint32_t

// =========================================================================
// Descriptor Set Indices (Corresponds to 'layout(set = X)' in GLSL/HLSL)
// =========================================================================
// These define the "groups" of resources that change at different frequencies
// or belong to different logical categories.
enum class DescriptorSetIndex : uint32_t
{
    GLOBAL_DATA_SET = 0,    // Data constant per-frame or per-scene (camera, global lights, environment maps)
    OBJECT_DATA_SET = 1,    // Data constant per-object instance (model matrix, object ID)
    MATERIAL_DATA_SET = 2,  // Data constant per-material (textures, material properties)


    PASS_DATA_SET = 3,      // Data specific to a rendering pass (e.g., post-processing inputs, shadow map atlas)
    // Add more sets as needed for specific engine features (e.g., UI, particles, bindless arrays)
    COUNT // Helper member for array sizes, if you have an array of descriptor sets
};

// =========================================================================
// Binding Slots within GLOBAL_DATA_SET (Corresponds to 'layout(binding = Y)' for set 0)
// =========================================================================
// Resources that are typically updated once per frame or are truly global.
enum class GlobalBindingSlot : uint32_t
{
    // Uniform Buffers (cbuffer in HLSL)
    CAMERA_UNIFORM_BUFFER = 0,       // Contains View, Projection, ViewProj matrices, camera position, etc.
    LIGHTS_UNIFORM_BUFFER = 1,       // Contains global light data (sun, ambient, number of active lights)
    TIME_UNIFORM_BUFFER = 2,         // Contains time-related variables (total time, delta time)
    SHADOW_DATA_UNIFORM_BUFFER = 3,  // Contains data for shadow mapping (light view-proj, cascade info)

    // Textures & Samplers
    ENVIRONMENT_MAP_CUBE = 4,        // Cube map for skybox/IBL environment
    IRRADIANCE_MAP_CUBE = 5,         // Irradiance cube map for diffuse IBL
    SPECULAR_PREFILTER_MAP_CUBE = 6, // Specular pre-filtered environment map for specular IBL
    BRDF_LUT_TEXTURE = 7,            // BRDF Look-Up Table for PBR specular
    SHADOW_MAP_ARRAY = 8,            // Texture array for cascaded shadow maps or omnidirectional shadows

    // Storage Buffers (StructuredBuffer/ByteAddressBuffer in HLSL)
    GLOBAL_ENTITY_DATA_SSBO = 9,     // Example: Large array of global entity properties

    COUNT // Helper member
};

// =========================================================================
// Binding Slots within OBJECT_DATA_SET (Corresponds to 'layout(binding = Y)' for set 1)
// =========================================================================
// Resources that change per object instance being drawn.
enum class ObjectBindingSlot : uint32_t
{
    MODEL_TRANSFORM_UNIFORM_BUFFER = 0, // Contains model matrix, normal matrix (inverse-transpose model)
    INSTANCE_DATA_UNIFORM_BUFFER = 1,   // If using instancing, holds per-instance attributes not in vertex buffer
    BONE_TRANSFORMS_UNIFORM_BUFFER = 2, // For skeletal animation (array of bone matrices)
    OBJECT_ID_BUFFER = 3,               // Example: A buffer containing unique object IDs for picking/selection

    COUNT // Helper member
};

// =========================================================================
// Binding Slots within MATERIAL_DATA_SET (Corresponds to 'layout(binding = Y)' for set 2)
// =========================================================================
// Resources that change per material.
enum class MaterialBindingSlot : uint32_t
{
    // Uniform Buffers
    PBR_MATERIAL_UNIFORM_BUFFER = 0, // Contains scalar/vector PBR factors (baseColorFactor, metallicFactor, roughnessFactor, etc.)
    CUSTOM_MATERIAL_UNIFORM_BUFFER = 1, // For materials with unique, custom properties

    // Textures & Samplers
	ALBEDO_MAP = 2,             // Base color texture , also known as diffuse or base color map
    NORMAL_MAP = 3,             // Normal map texture
    METALLIC_ROUGHNESS_MAP = 4, // Combined metallic/roughness/AO texture (e.g., GLTF standard)
    EMISSIVE_MAP = 5,           // Emissive light texture
    OCCLUSION_MAP = 6,          // Ambient occlusion map
    HEIGHT_MAP = 7,             // For parallax occlusion mapping/displacement

    // You might also define dedicated samplers if not using combined texture/sampler objects
    // or if a texture needs a specific sampler configuration that's not its default.
    // E.g., STATIC_CLAMP_SAMPLER = 8, DYNAMIC_LINEAR_SAMPLER = 9

    COUNT // Helper member
};

// =========================================================================
// Binding Slots within PASS_DATA_SET (Corresponds to 'layout(binding = Y)' for set 3)
// =========================================================================
// Resources specific to a particular rendering pass (e.g., G-buffer inputs for a deferred pass,
// post-processing effect inputs, specific light lists for a clustered renderer).
enum class PassBindingSlot : uint32_t
{
    // For deferred rendering:
    GBUFFER_ALBEDO_SPECULAR_TEXTURE = 0, // G-buffer attachment 0
    GBUFFER_NORMAL_ROUGHNESS_TEXTURE = 1, // G-buffer attachment 1
    GBUFFER_POSITION_TEXTURE = 2,       // G-buffer attachment 2 (or depth buffer as texture)
    GBUFFER_EMISSIVE_OCCLUSION_TEXTURE = 3, // G-buffer attachment 3 (if present)

    // For post-processing:
    POST_FX_INPUT_TEXTURE = 0,          // Main input texture for a post-process effect
    POST_FX_PARAMETERS_UNIFORM_BUFFER = 1, // Parameters specific to the post-process effect
    DEPTH_BUFFER_TEXTURE = 4,           // Often needed separately for depth-aware effects

    // For clustered lighting:
    CLUSTER_LIGHT_INDICES_SSBO = 5,     // Shader Storage Buffer Object for light indices per cluster
    CLUSTER_LIGHT_DATA_SSBO = 6,        // Shader Storage Buffer Object for actual light data

    COUNT // Helper member
};



/*USE GL_ARB_separate_shader_objects for set 0, 1, 2, and binding 0, 1, 2, etc.//nope false 

// OR USE GL_ARB_separate_shader_objects for set 0, 1, 2, and binding 0, 1, 2, etc.//nope false
// =========================================================================
// Global base offsets for OpenGL's unified binding points
namespace OpenGLBindingOffsets
{
    static constexpr uint32_t GLOBAL_UBO_BASE_BINDING_SLOT = 0;
    static constexpr uint32_t OBJECT_UBO_BASE_BINDING_SLOT = 10; // Reserve 0-9 for global
    static constexpr uint32_t MATERIAL_UBO_BASE_BINDING_SLOT = 20; // Reserve 10-19 for object

    static constexpr uint32_t GLOBAL_TEXTURE_BASE_BINDING_SLOT = 0; // Texture units often start at 0
    static constexpr uint32_t MATERIAL_TEXTURE_BASE_BINDING_SLOT = 10; // Reserve 0-9 for global textures
    // ... and so on for SSBOs
}

// Binding Slots within MATERIAL_DATA_SET (NOW RELATIVE TO ITS BASE OFFSET)
enum class MaterialBindingSlot : uint32_t
{
    PBR_MATERIAL_UNIFORM_BUFFER_RELATIVE = 0,
    ALBEDO_MAP_RELATIVE = 0, // Texture unit 0 within material's range
    NORMAL_MAP_RELATIVE = 1,
    // ...
};

// You'd use a function to get the final binding slot:
uint32_t getGLSLBinding(DescriptorSetIndex set, uint32_t relativeSlot)
{
    switch (set)
    {
    case DescriptorSetIndex::GLOBAL_DATA_SET: return OpenGLBindingOffsets::GLOBAL_UBO_BASE_BINDING_SLOT + relativeSlot;
    case DescriptorSetIndex::OBJECT_DATA_SET: return OpenGLBindingOffsets::OBJECT_UBO_BASE_BINDING_SLOT + relativeSlot;
    case DescriptorSetIndex::MATERIAL_DATA_SET: return OpenGLBindingOffsets::MATERIAL_UBO_BASE_BINDING_SLOT + relativeSlot;
        // ...
    }
    return 0; // Error or default
}



*/