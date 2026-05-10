#pragma once
#include <rendering-system/gpu-resource-system/data-structures/gpu_mesh_system_data_structures.h>

namespace TheEngine::RenderingSystem
{


	constexpr size_t getAttributeSize(const VertexAttributeBits attr);


	size_t calculateVertexStride(const VertexFormat format);

	size_t getVertexSize(const VertexFormat vertexFormat);

	size_t getOffsetOfVertexAttribute(const VertexFormat format, const VertexAttributeBits target);



}
