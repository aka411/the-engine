#include "rendering-engine/opengl-backend/opengl_texture_helper.h"
#include <cassert>



namespace TheEngine::OpenGLBackend
{


	GLenum toGLEnum(TextureSourcePixelFormat format)
	{
		switch (format)
		{
		case TextureSourcePixelFormat::R:     return GL_RED;
		case TextureSourcePixelFormat::RG:    return GL_RG;
		case TextureSourcePixelFormat::RGB:   return GL_RGB;
		case TextureSourcePixelFormat::RGBA:  return GL_RGBA;
		case TextureSourcePixelFormat::BGRA:  return GL_BGRA;
		default: return 0; // Handle error or unknown
		}
	}

	GLenum toGLEnum(TextureSourceComponentType type)
	{
		switch (type)
		{
		case TextureSourceComponentType::UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
		case TextureSourceComponentType::FLOAT:         return GL_FLOAT;
		case TextureSourceComponentType::HALF_FLOAT:    return GL_HALF_FLOAT;
		default: return 0;
		}
	}



	GLenum toGLEnum(TextureInternalFormat format)
	{
		switch (format)
		{
			// 8-bit Unsigned Integer Formats (Sized Formats)
		case TextureInternalFormat::R8:          return GL_R8;
		case TextureInternalFormat::RG8:         return GL_RG8;
		case TextureInternalFormat::RGB8:        return GL_RGB8;
		case TextureInternalFormat::RGBA8:       return GL_RGBA8;

			// 16-bit Half-Float Formats
		case TextureInternalFormat::R16F:        return GL_R16F;
		case TextureInternalFormat::RG16F:       return GL_RG16F;
		case TextureInternalFormat::RGB16F:      return GL_RGB16F;
		case TextureInternalFormat::RGBA16F:     return GL_RGBA16F;

			// 32-bit Full-Float Formats
		case TextureInternalFormat::R32F:        return GL_R32F;
		case TextureInternalFormat::RG32F:       return GL_RG32F;
		case TextureInternalFormat::RGB32F:      return GL_RGB32F;
		case TextureInternalFormat::RGBA32F:     return GL_RGBA32F;

			// Depth/Stencil formats
		case TextureInternalFormat::DEPTH_COMPONENT16: return GL_DEPTH_COMPONENT16;
		case TextureInternalFormat::DEPTH_COMPONENT24: return GL_DEPTH_COMPONENT24;
		case TextureInternalFormat::DEPTH_COMPONENT32F: return GL_DEPTH_COMPONENT32F;
		case TextureInternalFormat::DEPTH_STENCIL: return GL_DEPTH_STENCIL;

		case TextureInternalFormat::UNKNOWN:
		default:
			assert(0);
			return 0;
		}
	}







	GLenum toGLEnum(TextureType type)
	{
		switch (type)
		{
		case TextureType::TEXTURE_2D:       return GL_TEXTURE_2D;
		case TextureType::TEXTURE_3D:       return GL_TEXTURE_3D;
		case TextureType::TEXTURE_CUBE_MAP: return GL_TEXTURE_CUBE_MAP;
		case TextureType::TEXTURE_2D_ARRAY: return GL_TEXTURE_2D_ARRAY;
		default: return 0; // Unknown or error type
		}
	}


	//------- SAMPLER --------//


	GLenum toGLEnum(TextureFilter filter, bool isMagFilter )
	{
		if (isMagFilter)
		{
			// Magnification filter cannot use mipmapping options
			switch (filter)
			{
			case TextureFilter::NEAREST:
			case TextureFilter::NEAREST_MIPMAP_NEAREST:
			case TextureFilter::NEAREST_MIPMAP_LINEAR:
				return GL_NEAREST; // Mag filter must be GL_NEAREST or GL_LINEAR

			case TextureFilter::LINEAR:
			case TextureFilter::LINEAR_MIPMAP_NEAREST:
			case TextureFilter::LINEAR_MIPMAP_LINEAR:
				return GL_LINEAR; // Mag filter must be GL_NEAREST or GL_LINEAR

			default: return GL_LINEAR; // Default safe value
			}
		}
		else
		{
			// Minification filter uses the full range of options
			switch (filter)
			{
			case TextureFilter::NEAREST: 				return GL_NEAREST;
			case TextureFilter::LINEAR: 				return GL_LINEAR;
			case TextureFilter::NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
			case TextureFilter::LINEAR_MIPMAP_NEAREST: 	return GL_LINEAR_MIPMAP_NEAREST;
			case TextureFilter::NEAREST_MIPMAP_LINEAR: 	return GL_NEAREST_MIPMAP_LINEAR;
			case TextureFilter::LINEAR_MIPMAP_LINEAR: 	return GL_LINEAR_MIPMAP_LINEAR;
			default: return GL_LINEAR_MIPMAP_LINEAR;
			}

		}
	}

	GLenum toGLEnum(TextureWrap wrap)
	{
		switch (wrap)
		{
		case TextureWrap::REPEAT: 			return GL_REPEAT;
		case TextureWrap::CLAMP_TO_EDGE: 	return GL_CLAMP_TO_EDGE;
		case TextureWrap::MIRRORED_REPEAT: 	return GL_MIRRORED_REPEAT;
			// You could add CLAMP_TO_BORDER here if needed
		default: return GL_REPEAT; // Default safe value
		}
	}

	bool requiresMipmaps(TextureFilter minFilter)
	{
		return (minFilter == TextureFilter::NEAREST_MIPMAP_NEAREST ||
			minFilter == TextureFilter::LINEAR_MIPMAP_NEAREST ||
			minFilter == TextureFilter::NEAREST_MIPMAP_LINEAR ||
			minFilter == TextureFilter::LINEAR_MIPMAP_LINEAR);
	}


	size_t getBytesPerPixel(TextureInternalFormat format)
	{
		switch (format)
		{
		case TextureInternalFormat::R8:          return 1;
		case TextureInternalFormat::RG8:         return 2;
		case TextureInternalFormat::RGB8:        return 3;
		case TextureInternalFormat::RGBA8:       return 4;
		case TextureInternalFormat::R16F:        return 2;
		case TextureInternalFormat::RG16F:       return 4;
		case TextureInternalFormat::RGB16F:      return 6;
		case TextureInternalFormat::RGBA16F:     return 8;
		case TextureInternalFormat::R32F:        return 4;
		case TextureInternalFormat::RG32F:       return 8;
		case TextureInternalFormat::RGB32F:      return 12;
		case TextureInternalFormat::RGBA32F:     return 16;
		default:
			assert(false && "Unknown TextureInternalFormat in getBytesPerPixel");
			return 0;
		}
	}

}