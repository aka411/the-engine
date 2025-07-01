#pragma once


// consider this later in design
//using ShaderBindingIndex = uint32_t;
//using ShaderDescriptorSetIndex = uint32_t;
// consider this later in design

enum class VertexAttributeSemantic
{
    POSITION,
    NORMAL,

    TEXCOORD_0,
    TEXCOORD_1,

    COLOR_0,
    COLOR_1


};

enum class PrimitiveTopology
{
    TRIANGLE_LIST,
    TRIANGLE_STRIP,

    LINE_LIST,
    LINE_STRIP,

    POINT_LIST,
};


enum class VertexAttributeFormat
{
    FLOAT1,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    INT1,
    INT2,
    INT3,
    INT4,
    UINT1,
    UINT2,
    UINT3,
    UINT4,

    HalfFloat2, // for 16-bit floats
    HalfFloat4,
    UNorm8_4, // for normalized unsigned 8-bit, 4 components (e.g., color)
    SNorm8_4, // for normalized signed 8-bit, 4 components
};




enum class IndexFormat
{
    NONE,
    UINT16,
    UINT32
};

