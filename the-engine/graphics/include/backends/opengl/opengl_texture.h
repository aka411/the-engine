#pragma once
#include "../../igpu_texture.h"

namespace TheEngine::Graphics
{


	struct OpenGLTextureCreateInfo
	{
		GLenum target;
		GLuint textureId;

		GLenum pixelFormat ;
		GLenum pixelDataType;

		GLenum internalFormat;

		
	};



	class OpenGLTexture : public IGPUTexture
	{
	private:
		const OpenGLTextureCreateInfo m_openGLTextureCreateInfo;

	public:
		OpenGLTexture(const  OpenGLTextureCreateInfo& createInfo);
			

		virtual TextureType GetType() const override;
		virtual TextureFormat GetFormat()const override;
		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight()const override;
		virtual uint32_t GetDepth() const override;       // For 3D textures
		virtual uint32_t GetMipLevels() const override;
		virtual uint32_t GetArrayLayers()const override; // For array textures/cubemaps
		virtual TextureUsageFlags GetUsage() const override;


		// NEW: OpenGL-specific getters for GLenum formats/types
		GLenum getGLInternalFormat() const { return m_glInternalFormat; }
		GLenum getGLFormatEnum() const { return m_glFormat; } // Renamed to avoid confusion with GetFormat()
		GLenum getGLTypeEnum() const { return m_glType; }     // Renamed to avoid confusion with GetType()


		GLuint getNativeHandle() const;
	

	};

}
