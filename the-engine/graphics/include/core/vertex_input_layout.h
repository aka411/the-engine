#pragma once
#include "common_enums.h"
#include <cstdint>
#include <vector>


namespace TheEngine::Graphics
{




    // Describes a single attribute within a vertex buffer layout
    struct VertexAttributeDescription // used by pso
    {
        VertexAttributeSemantic semantic; // e.g., Position, Normal, TexCoord0
        VertexAttributeFormat format;     // e.g., Float3, UNorm8_4
        uint32_t location;                // Shader input location (layout(location=X) in GLSL, or register)
        uint32_t offset;                  // Offset within the vertex structure (in bytes)
        uint32_t binding;                 // Which vertex buffer binding this attribute comes from//shader dependent
    };

    // Describes the overall layout of vertex data for a single vertex buffer binding
    struct VertexBindingDescription
    {
        uint32_t binding;           // Unique identifier for this binding (e.g., slot 0, slot 1)//shader dependent
        uint32_t stride;            // Size of a single vertex (in bytes)
        // Could also add InputRate (per-vertex or per-instance)
        // enum class VertexInputRate { Vertex, Instance };
        // VertexInputRate inputRate;

    };

    // Overall vertex input layout for a pipeline
    struct VertexInputLayout
    {
        std::vector<VertexBindingDescription> bindings;
        std::vector<VertexAttributeDescription> attributes;

    };


}