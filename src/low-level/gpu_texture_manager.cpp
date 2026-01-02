

#include <stdexcept>
#include <cassert>

#include "low-level/rendering_system_data_types.h"
#include "low-level/gpu_texture_manager.h"
#include "glad/glad.h"
#include <iostream>
#include "rendering-engine/opengl-backend/opengl_texture_helper.h"








namespace TheEngine
{




	GPUTextureManager::GPUTextureManager()
	{


	}

	GPUTextureManager::~GPUTextureManager()
	{
		//destroying texture handles
			//glDeleteTextures(m_allotedGLTextures.size(), m_allotedGLTextures.data());


		//TODO : destroy any more new type of resources also


	}

	static std::string GLErrorToString(GLenum error)
	{
		switch (error)
		{
		case GL_INVALID_ENUM: return "INVALID_ENUM";
		case GL_INVALID_VALUE: return "INVALID_VALUE";
		case GL_INVALID_OPERATION: return "INVALID_OPERATION";
		case GL_STACK_OVERFLOW: return "STACK_OVERFLOW";
		case GL_OUT_OF_MEMORY: return "OUT_OF_MEMORY";
		case 0: return "NO_ERROR";
		default: return "UNKNOWN_ERROR";
		}
	}


	TextureInfo GPUTextureManager::createNewTexture(const TextureCreateInfo& textureCreateInfo)
	{



		GLenum glInternalFormat = OpenGLBackend::toGLEnum(textureCreateInfo.internalFormat);
		GLenum glSourceFormat = OpenGLBackend::toGLEnum(textureCreateInfo.textureSourcePixelFormat);
		GLenum glSourceType = OpenGLBackend::toGLEnum(textureCreateInfo.textureSourceComponentType);


		GLenum target = OpenGLBackend::toGLEnum(textureCreateInfo.type);

		SamplerSetting samplerSettings = textureCreateInfo.samplerSetting;


		GLuint glTextureId;
		glCreateTextures(GL_TEXTURE_2D, 1, &glTextureId);
		glBindTexture(target, glTextureId);


		/*
		// Special handling for 1-channel textures (like R/Luminance) to prevent padding issues
		if (textureCreateInfo.textureSourcePixelFormat == TextureSourcePixelFormat::R)
		{
			// OpenGL defaults to 4-byte alignment; this forces 1-byte alignment for R-only data.
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		}
		else
		{
			// Restore default alignment for multi-channel data
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}
		*/


		// 4. **Upload Data (The core GL call)**
		// This single call handles all formats/types using the mapped enums
		glTexImage2D(
			target,
			0,                                 // Mipmap level
			glInternalFormat,                  // e.g., GL_RGBA8 (GPU Storage)
			textureCreateInfo.width,
			textureCreateInfo.height,
			0,                                 // Border (must be 0)
			glSourceFormat,                    // e.g., GL_RGB (CPU Data Layout)
			glSourceType,                      // e.g., GL_UNSIGNED_BYTE (CPU Component Type)
			(void*)textureCreateInfo.data             // Raw pixel data
		);


		GLErrorToString(glGetError());


		bool hasMipmaps = OpenGLBackend::requiresMipmaps(samplerSettings.minFilter);

		//if (hasMipmaps)
		//{
		glGenerateMipmap(target);
		//}




		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, OpenGLBackend::toGLEnum(samplerSettings.minFilter));
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, OpenGLBackend::toGLEnum(samplerSettings.magFilter));
		glTexParameteri(target, GL_TEXTURE_WRAP_S, OpenGLBackend::toGLEnum(samplerSettings.wrapS));
		glTexParameteri(target, GL_TEXTURE_WRAP_T, OpenGLBackend::toGLEnum(samplerSettings.wrapT));





		GLuint64 residentHandle = 0;


		residentHandle = glGetTextureHandleARB(glTextureId);

		assert(residentHandle != 0);
		glMakeTextureHandleResidentARB(residentHandle);

		//std::cout<<GLErrorToString(glGetError())<<std::endl;


	//m_allotedGLTextureIDs.push_back(glTextureId); // For destruction tracking




		TextureInfo info;
		//info.textureHandle = glTextureId;
		info.resisdentHandle = residentHandle;
		info.type = textureCreateInfo.type;
		info.width = textureCreateInfo.width;
		info.height = textureCreateInfo.height;
		info.internalFormat = textureCreateInfo.internalFormat;
		info.samplerSettings = samplerSettings;
		info.hasMipmaps = hasMipmaps;


		m_residentHandleToTextureApiHandle.emplace(residentHandle, glTextureId);// info);
		m_totalAllocatedTextureMemory += textureCreateInfo.width * textureCreateInfo.height * OpenGLBackend::getBytesPerPixel(textureCreateInfo.internalFormat);

		return info;


	}

	TextureInfo GPUTextureManager::createBufferTexture(const BufferTextureCreateInfo& bufferTextureCreateInfo)
	{





		return TextureInfo();
	}



	void GPUTextureManager::destroyTexture(TextureInfo& textureInfo)
	{

		const uint64_t residentHandle = textureInfo.resisdentHandle;
		auto it = m_residentHandleToTextureApiHandle.find(residentHandle);

		if (it == m_residentHandleToTextureApiHandle.end())
		{
			// Handle error: texture not found
			return;
		}

		GLuint glTextureId = it->second;

		// 2. Make the 64-bit handle non-resident
		glMakeTextureHandleNonResidentARB(residentHandle);


		glDeleteTextures(1, &glTextureId);


		m_residentHandleToTextureApiHandle.erase(it);


	}


	size_t GPUTextureManager::getTotalAllocatedTextureMemory() const
	{
		return m_totalAllocatedTextureMemory;
	}


}