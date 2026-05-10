#include <rendering-system/gpu-resource-system/utils/mesh_system_helpers.h>


namespace TheEngine::RenderingSystem
{


	constexpr size_t getAttributeSize(const VertexAttributeBits attr)
	{
		switch (attr)
		{
		case VertexAttributeBits::POSITION: return sizeof(float) * 3;
		case VertexAttributeBits::NORMAL:   return sizeof(float) * 3;
		case VertexAttributeBits::COLOR:    return sizeof(float) * 4;
		case VertexAttributeBits::UV_0:     return sizeof(float) * 2;
		case VertexAttributeBits::UV_1:     return sizeof(float) * 2;
		case VertexAttributeBits::TANGENT:  return sizeof(float) * 4;
		case VertexAttributeBits::WEIGHTS:  return sizeof(float) * 4;
		case VertexAttributeBits::JOINTS:   return sizeof(uint32_t) * 4;

		case VertexAttributeBits::NONE:
		default: 
			assert(false && "Unknown VertexAttributeBits");
			return 0;

		}
	}

	size_t calculateVertexStride(const VertexFormat format)
	{
		size_t stride = 0;
		uint16_t mask = static_cast<uint16_t>(format);


		for (int i = 0; i < 16; ++i)
		{
			uint16_t bit = 1 << i;
			if (mask & bit)
			{
				stride += getAttributeSize(static_cast<VertexAttributeBits>(bit));
			}
		}
		return stride;
	}


	size_t getVertexSize(const VertexFormat vertexFormat)
	{

		return calculateVertexStride(vertexFormat);//FOR NOW SINCE ALL DATA IS INTERLEAVED ITS SAME AS STRIDE

	}


	size_t getOffsetOfVertexAttribute(const VertexFormat format, const VertexAttributeBits target) 
	{
		size_t offset = 0;
		uint16_t mask = static_cast<uint16_t>(format);
		uint16_t targetBit = static_cast<uint16_t>(target);

		assert((mask & targetBit) && "Target VertexAttributeBits not present in VertexFormat");

		for (int i = 0; i < 16; ++i) 
		{
			const uint16_t currentBit = 1 << i;

		
			if (currentBit == targetBit) 
			{
				return  offset; 
			}

		
			if (mask & currentBit) 
			{
				offset += getAttributeSize(static_cast<VertexAttributeBits>(currentBit));
			}
		}

		return offset;
	}



}
