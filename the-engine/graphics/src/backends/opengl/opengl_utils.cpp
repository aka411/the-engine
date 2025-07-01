#include "../../../include/backends/opengl/opengl_utils.h"

namespace TheEngine::Graphics::OpenGLUtils
{


	//mostly cleaned

	GLenum getGLInternalFormat(const TextureFormat& format)
	{
		switch (format)
		{
			// UNORM

		case TextureFormat::RGB8_UNORM:     return GL_RGB8; // Often becomes GL_RGBA8 on modern hardware
		case TextureFormat::RGBA8_UNORM:    return GL_RGBA8;
		case TextureFormat::RGBA16_UNORM:   return GL_RGBA16;



			// SNORM
		case TextureFormat::R8_SNORM:       return GL_R8_SNORM;
		case TextureFormat::RG8_SNORM:      return GL_RG8_SNORM;
		case TextureFormat::RGBA8_SNORM:    return GL_RGBA8_SNORM;
		case TextureFormat::RG16_SNORM:     return GL_RG16_SNORM;
		case TextureFormat::RGBA16_SNORM:   return GL_RGBA16_SNORM;

			// UINT

		case TextureFormat::RGBA8_UINT:     return GL_RGBA8UI;
		case TextureFormat::RGBA16_UINT:    return GL_RGBA16UI;
		case TextureFormat::RGBA32_UINT:    return GL_RGBA32UI;

			// SINT

		case TextureFormat::RGBA8_SINT:     return GL_RGBA8I;
		case TextureFormat::RGBA16_SINT:    return GL_RGBA16I;
		case TextureFormat::RGBA32_SINT:    return GL_RGBA32I;

			// FLOAT

		case TextureFormat::RGB16_FLOAT:    return GL_RGB16F;
		case TextureFormat::RGB32_FLOAT:    return GL_RGB32F;
		case TextureFormat::RGBA16_FLOAT:   return GL_RGBA16F;
		case TextureFormat::RGBA32_FLOAT:   return GL_RGBA32F;

			// sRGB
		case TextureFormat::RGBA8_SRGB:     return GL_SRGB8_ALPHA8;
		case TextureFormat::BGRA8_SRGB:     return GL_SRGB8_ALPHA8; // BGRA often becomes RGBA on GPU, then swizzled

			// Depth & Stencil
		case TextureFormat::D16_UNORM:          return GL_DEPTH_COMPONENT16;
		case TextureFormat::D32_FLOAT:          return GL_DEPTH_COMPONENT32F;
		case TextureFormat::D24_UNORM_S8_UINT:  return GL_DEPTH24_STENCIL8;
		case TextureFormat::D32_FLOAT_S8_UINT:  return GL_DEPTH32F_STENCIL8;

		default:
			throw std::runtime_error("Unsupported texture format for OpenGL internal format mapping.");
		}
	}



	// Maps generic TextureFormat to OpenGL's pixel data format (for glTextureSubImage2D)
	GLenum getGLPixelDataFormat(const TextureFormat& format)
	{
		switch (format)
		{
		case TextureFormat::R8_UNORM:
		case TextureFormat::R16_UNORM:
		case TextureFormat::R8_SNORM:
		case TextureFormat::R8_UINT:
		case TextureFormat::R8_SINT:
		case TextureFormat::R16_FLOAT:
		case TextureFormat::R32_FLOAT:
		case TextureFormat::R16_UINT:
		case TextureFormat::R16_SINT:
		case TextureFormat::R32_UINT:
		case TextureFormat::R32_SINT:
			return GL_RED;

		case TextureFormat::RG8_UNORM:
		case TextureFormat::RG16_UNORM:
		case TextureFormat::RG8_SNORM:
		case TextureFormat::RG16_SNORM:
		case TextureFormat::RG8_UINT:
		case TextureFormat::RG16_UINT:
		case TextureFormat::RG32_UINT:
		case TextureFormat::RG8_SINT:
		case TextureFormat::RG16_SINT:
		case TextureFormat::RG32_SINT:
		case TextureFormat::RG16_FLOAT:
		case TextureFormat::RG32_FLOAT:
			return GL_RG;

		case TextureFormat::RGB8_UNORM:
		case TextureFormat::RGB8_SNORM:
		case TextureFormat::RGB16_FLOAT:
		case TextureFormat::RGB32_FLOAT:
			return GL_RGB;

		case TextureFormat::RGBA8_UNORM:
		case TextureFormat::RGBA16_UNORM:
		case TextureFormat::RGBA8_SNORM:
		case TextureFormat::RGBA16_SNORM:
		case TextureFormat::RGB10A2_UNORM:
		case TextureFormat::RGBA8_UINT:
		case TextureFormat::RGBA16_UINT:
		case TextureFormat::RGBA32_UINT:
		case TextureFormat::RGBA8_SINT:
		case TextureFormat::RGBA16_SINT:
		case TextureFormat::RGBA32_SINT:
		case TextureFormat::RGBA16_FLOAT:
		case TextureFormat::RGBA32_FLOAT:
		case TextureFormat::RGBA8_SRGB:
			return GL_RGBA;

		case TextureFormat::BGRA8_UNORM:
		case TextureFormat::BGRA8_SRGB:
			return GL_BGRA; // Specific for BGRA input data

			// Depth & Stencil
		case TextureFormat::D16_UNORM:
		case TextureFormat::D32_FLOAT:
			return GL_DEPTH_COMPONENT;

		case TextureFormat::D24_UNORM_S8_UINT:
		case TextureFormat::D32_FLOAT_S8_UINT:
			return GL_DEPTH_STENCIL;

		default:
			throw std::runtime_error("Unsupported texture format for OpenGL pixel data format mapping.");
		}
	}


	// Maps generic TextureFormat to OpenGL's pixel data type (for glTextureSubImage2D)
	GLenum getGLPixelDataType(const TextureFormat& format)
	{
		switch (format)
		{
		case TextureFormat::R8_UNORM:
		case TextureFormat::RG8_UNORM:
		case TextureFormat::RGB8_UNORM:
		case TextureFormat::RGBA8_UNORM:
		case TextureFormat::BGRA8_UNORM:
		case TextureFormat::RGBA8_SRGB:
		case TextureFormat::BGRA8_SRGB:
			return GL_UNSIGNED_BYTE;

		case TextureFormat::R8_SNORM:
		case TextureFormat::RG8_SNORM:
		case TextureFormat::RGB8_SNORM:
		case TextureFormat::RGBA8_SNORM:
			return GL_BYTE; // Signed byte

		case TextureFormat::R16_UNORM:
		case TextureFormat::RG16_UNORM:
		case TextureFormat::RGBA16_UNORM:
			return GL_UNSIGNED_SHORT;

		case TextureFormat::R16_SNORM:
		case TextureFormat::RG16_SNORM:
		case TextureFormat::RGBA16_SNORM:
			return GL_SHORT; // Signed short

		case TextureFormat::R32_UNORM: // If used, typically unsigned int
		case TextureFormat::RG32_UNORM:
			return GL_UNSIGNED_INT;

		case TextureFormat::R8_UINT:
		case TextureFormat::RG8_UINT:
		case TextureFormat::RGBA8_UINT:
			return GL_UNSIGNED_BYTE;

		case TextureFormat::R16_UINT:
		case TextureFormat::RG16_UINT:
		case TextureFormat::RGBA16_UINT:
			return GL_UNSIGNED_SHORT;

		case TextureFormat::R32_UINT:
		case TextureFormat::RG32_UINT:
		case TextureFormat::RGBA32_UINT:
			return GL_UNSIGNED_INT;

		case TextureFormat::R8_SINT:
		case TextureFormat::RG8_SINT:
		case TextureFormat::RGBA8_SINT:
			return GL_BYTE;

		case TextureFormat::R16_SINT:
		case TextureFormat::RG16_SINT:
		case TextureFormat::RGBA16_SINT:
			return GL_SHORT;

		case TextureFormat::R32_SINT:
		case TextureFormat::RG32_SINT:
		case TextureFormat::RGBA32_SINT:
			return GL_INT;

		case TextureFormat::R16_FLOAT:
		case TextureFormat::RG16_FLOAT:
		case TextureFormat::RGB16_FLOAT:
		case TextureFormat::RGBA16_FLOAT:
			return GL_HALF_FLOAT; // Typically half-precision float

		case TextureFormat::R32_FLOAT:
		case TextureFormat::RG32_FLOAT:
		case TextureFormat::RGB32_FLOAT:
		case TextureFormat::RGBA32_FLOAT:
		case TextureFormat::D32_FLOAT:
			return GL_FLOAT;

		case TextureFormat::RGB10A2_UNORM:
			return GL_UNSIGNED_INT_2_10_10_10_REV; // Packed format

		case TextureFormat::D16_UNORM:
			return GL_UNSIGNED_SHORT;
		case TextureFormat::D24_UNORM_S8_UINT:
			return GL_UNSIGNED_INT_24_8; // Packed format
		case TextureFormat::D32_FLOAT_S8_UINT:
			return GL_FLOAT_32_UNSIGNED_INT_24_8_REV; // Packed float depth, int stencil

		default:
			throw std::runtime_error("Unsupported texture format for OpenGL pixel data type mapping.");
		}
	}




	GLenum getGLTextureTarget(const TextureType& type)
	{
		switch (type)
		{
		case TextureType::TEXTURE_1D:
			return GL_TEXTURE_1D;
		case TextureType::TEXTURE_2D:
			return GL_TEXTURE_2D;
		case TextureType::TEXTURE_3D:
			return GL_TEXTURE_3D;
		default:
			throw std::runtime_error("Unsupported texture type for OpenGL.");
		}
	}




	GLbitfield getGLMemoryAccessFlags(const BufferDescriptor& bufferDescriptor)
	{
		GLbitfield flags = 0;
		if (bufferDescriptor.memoryAccessFlag & MemoryAccessFlags::COHERENT_MAPPING)
		{
			flags |= GL_MAP_COHERENT_BIT;
		}
		if (bufferDescriptor.memoryAccessFlag & MemoryAccessFlags::PERSISTENT_MAPPING)
		{
			flags |= GL_MAP_PERSISTENT_BIT;
		}
		if (bufferDescriptor.memoryAccessFlag & MemoryAccessFlags::CLIENT_STORAGE)
		{
			flags |= GL_CLIENT_STORAGE_BIT;
		}
		if (bufferDescriptor.memoryAccessFlag & MemoryAccessFlags::DYNAMIC_STORAGE)
		{
			flags |= GL_DYNAMIC_STORAGE_BIT;
		}
		return flags;
	}

	GLenum getGLTarget(const BufferDescriptor& bufferDescriptor)
	{
		switch (bufferDescriptor.usage)
		{
		case BufferUsage::VERTEX:
			return GL_ARRAY_BUFFER;
		case BufferUsage::INDEX:
			return GL_ELEMENT_ARRAY_BUFFER;
		case BufferUsage::UNIFORM:
			return GL_UNIFORM_BUFFER;

		default:
			return 0; // Unsupported usage
		}
	}


	GLenum getMagFilter(const FilterMode& filterMode)
	{
		switch (filterMode)
		{
		case FilterMode::LINEAR:
			return GL_LINEAR;
		case FilterMode::NEAREST:
			return GL_NEAREST;
		default:
			throw std::runtime_error("Unsupported filter mode for OpenGL.");
		}
	}

	GLenum getMinFilter(const FilterMode& filterMode, const MipmapMode& mipmapMode)
	{
		switch (filterMode)
		{
		case FilterMode::LINEAR:
			switch (mipmapMode)
			{
			case MipmapMode::LINEAR:
				return GL_LINEAR_MIPMAP_LINEAR;
			case MipmapMode::NEAREST:
				return GL_LINEAR_MIPMAP_NEAREST;
			case MipmapMode::NONE:
				return GL_LINEAR; // No mipmapping, just linear filtering
			default:
				throw std::runtime_error("Unsupported mipmap mode for OpenGL.");
			}
		case FilterMode::NEAREST:
			switch (mipmapMode)
			{
			case MipmapMode::LINEAR:
				return GL_NEAREST_MIPMAP_LINEAR;
			case MipmapMode::NEAREST:
				return GL_NEAREST_MIPMAP_NEAREST;
			case  MipmapMode::NONE:
				return GL_NEAREST;// No mipmapping, just nearest filtering
			default:
				throw std::runtime_error("Unsupported mipmap mode for OpenGL.");
			}
		}
	}

	GLenum getGLAddressMode(const AddressMode& addressMode)
	{
		switch (addressMode)
		{
		case AddressMode::REPEAT:
			return GL_REPEAT;
		case AddressMode::CLAMP_TO_EDGE:
			return GL_CLAMP_TO_EDGE;
		case AddressMode::MIRRORED_REPEAT:
			return GL_MIRRORED_REPEAT;
		case AddressMode::CLAMP_TO_BORDER:
			return GL_CLAMP_TO_BORDER;
		case AddressMode::MIRROR_CLAMP_TO_EDGE:
			return GL_MIRROR_CLAMP_TO_EDGE;
		case AddressMode::NONE: // No addressing mode
			return GL_REPEAT; //fallback,
			//in vulkan we would need to set s,t,w wrap even if we do not need w.
		default:
			throw std::runtime_error("Unsupported address mode for OpenGL.");
		}

	}

	GLenum getGLVertexAttributeType(const VertexAttributeFormat& format)
	{
		switch (format)
		{
		case VertexAttributeFormat::FLOAT1:
			return GL_FLOAT;
		case VertexAttributeFormat::FLOAT2:
			return GL_FLOAT;
		case VertexAttributeFormat::FLOAT3:
			return GL_FLOAT;
		case VertexAttributeFormat::FLOAT4:
			return GL_FLOAT;

		case VertexAttributeFormat::INT1:
			return GL_INT;
		case VertexAttributeFormat::INT2:
			return GL_INT;
		case VertexAttributeFormat::INT3:
			return GL_INT;
		case VertexAttributeFormat::INT4:
			return GL_INT;

		case VertexAttributeFormat::UINT1:
			return GL_UNSIGNED_INT;
		case VertexAttributeFormat::UINT2:
			return GL_UNSIGNED_INT;
		case VertexAttributeFormat::UINT3:
			return GL_UNSIGNED_INT;
		case VertexAttributeFormat::UINT4:
			return GL_UNSIGNED_INT;

		case VertexAttributeFormat::HalfFloat2:
			return GL_HALF_FLOAT;
		case VertexAttributeFormat::HalfFloat4:
			return GL_HALF_FLOAT;

		case VertexAttributeFormat::UNorm8_4:
			return GL_UNSIGNED_BYTE; // Normalized unsigned 8-bit, 4 components

		case VertexAttributeFormat::SNorm8_4:
			return GL_BYTE; // Normalized signed 8-bit, 4 components
		default:
			throw std::runtime_error("Unsupported vertex attribute format for OpenGL.");
		}



	}

	GLint getGLVertexAttributeTypeSize(const VertexAttributeFormat& format)
	{
		switch (format)
		{
		case VertexAttributeFormat::FLOAT1:
			return sizeof(float);
		case VertexAttributeFormat::FLOAT2:
			return sizeof(float) * 2;
		case VertexAttributeFormat::FLOAT3:
			return sizeof(float) * 3;
		case VertexAttributeFormat::FLOAT4:
			return sizeof(float) * 4;
		case VertexAttributeFormat::INT1:
			return sizeof(int);
		case VertexAttributeFormat::INT2:
			return sizeof(int) * 2;
		case VertexAttributeFormat::INT3:
			return sizeof(int) * 3;
		case VertexAttributeFormat::INT4:
			return sizeof(int) * 4;
		case VertexAttributeFormat::UINT1:
			return sizeof(unsigned int);
		case VertexAttributeFormat::UINT2:
			return sizeof(unsigned int) * 2;
		case VertexAttributeFormat::UINT3:
			return sizeof(unsigned int) * 3;
		case VertexAttributeFormat::UINT4:
			return sizeof(unsigned int) * 4;
		case VertexAttributeFormat::HalfFloat2:
			return sizeof(GLhalf) * 2; // Half float is typically 16 bits
		case VertexAttributeFormat::HalfFloat4:
			return sizeof(GLhalf) * 4; // Half float is typically 16 bits
		case VertexAttributeFormat::UNorm8_4:
			return sizeof(unsigned char) * 4; // 8-bit normalized, 4 components
		case VertexAttributeFormat::SNorm8_4:
			return sizeof(signed char) * 4; // 8-bit normalized, 4 components
		default:
			throw std::runtime_error("Unsupported vertex attribute format for OpenGL size calculation.");
		}
	}

	GLboolean getGLVertexAttributeNormalized(const VertexAttributeFormat& format)
	{
		//need work on this
		switch (format)
		{
		case VertexAttributeFormat::UNorm8_4:
			return GL_TRUE; // Normalized unsigned 8-bit, 4 components
		case VertexAttributeFormat::SNorm8_4:
			return GL_TRUE; // Normalized signed 8-bit, 4 components
		case VertexAttributeFormat::HalfFloat2:
		case VertexAttributeFormat::HalfFloat4:
			return GL_FALSE; // Half float is not normalized
		default:
			return GL_FALSE; // Other formats are not normalized
		}
	}

	GLenum getGLIndexFormat(const IndexFormat& format)
	{
		switch (format)
		{
		case IndexFormat::UINT16:
			return GL_UNSIGNED_SHORT;
		case IndexFormat::UINT32:
			return GL_UNSIGNED_INT;
		case IndexFormat::NONE://consider removing this
			return 0; // No index format

		default:
			throw std::runtime_error("Unsupported index format for OpenGL.");
		}
	}

	GLenum getGLPrimitiveTopology(const PrimitiveTopology& topology)
	{

		switch (topology)
		{

		case PrimitiveTopology::POINT_LIST:
			return GL_POINTS;
		case PrimitiveTopology::LINE_LIST:
			return GL_LINES;
		case PrimitiveTopology::LINE_STRIP:
			return GL_LINE_STRIP;
		case PrimitiveTopology::TRIANGLE_LIST:
			return GL_TRIANGLES;
		default:
			std::runtime_error("Unsupported primitive topology for OpenGL.");

		}
	}

	GLenum getGLCullMode(const CullMode& cullMode)
	{
		switch (cullMode)
		{
		case CullMode::None:
			return GL_NONE; // No culling
		case CullMode::Back:
			return GL_BACK; // Cull back faces
		case CullMode::Front:
			return GL_FRONT; // Cull front faces
		case CullMode::FrontAndBack:
			return GL_FRONT_AND_BACK; // Cull both front and back faces
		default:
			throw std::runtime_error("Unsupported cull mode for OpenGL.");
		}

	}

	GLenum getGLPolygonMode(const PolygonMode& polygonMode)
	{
		switch (polygonMode)
		{
		case PolygonMode::Fill:
			return GL_FILL; // Fill mode
		case PolygonMode::Line:
			return GL_LINE; // Line mode
		case PolygonMode::Point:
			return GL_POINT; // Point mode
		default:
			throw std::runtime_error("Unsupported polygon mode for OpenGL.");
		}
	}







	

} // namespace TheEngine::Graphics::OpenGLUtils