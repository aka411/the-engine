

#include <stdexcept>
#include <cassert>

#include "low-level/rendering_system_data_types.h"
#include "low-level/gpu_texture_manager.h"
#include "glad/glad.h"
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



	TextureInfo GPUTextureManager::createNewTexture(const TextureCreateInfo& textureCreateInfo)
	{

		//ToDo : add methods for 3D also
		//Issue : We generate sampler per texture , we later implement method to reuse sampler
		GLenum glInternalFormat = OpenGLBackend::toGLEnum(textureCreateInfo.internalFormat);
		GLenum glSourceFormat = OpenGLBackend::toGLEnum(textureCreateInfo.textureSourcePixelFormat);
		GLenum glSourceType = OpenGLBackend::toGLEnum(textureCreateInfo.textureSourceComponentType);


		GLenum target = OpenGLBackend::toGLEnum(textureCreateInfo.type);

		const SamplerSetting& samplerSettings = textureCreateInfo.samplerSetting;


		GLuint glTextureId;
		glCreateTextures(GL_TEXTURE_2D, 1, &glTextureId);
	

		GLuint samplerId;
		glCreateSamplers(1, &samplerId);

		glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, OpenGLBackend::toGLEnum(textureCreateInfo.samplerSetting.minFilter));
		glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, OpenGLBackend::toGLEnum(textureCreateInfo.samplerSetting.magFilter));
		glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_S, OpenGLBackend::toGLEnum(textureCreateInfo.samplerSetting.wrapS));
		glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_T, OpenGLBackend::toGLEnum(textureCreateInfo.samplerSetting.wrapT));


		/*
		glTextureParameteri(glTextureId, GL_TEXTURE_MIN_FILTER, OpenGLBackend::toGLEnum(textureCreateInfo.samplerSetting.minFilter));
		glTextureParameteri(glTextureId, GL_TEXTURE_MAG_FILTER, OpenGLBackend::toGLEnum(textureCreateInfo.samplerSetting.magFilter));
		glTextureParameteri(glTextureId, GL_TEXTURE_WRAP_S, OpenGLBackend::toGLEnum(textureCreateInfo.samplerSetting.wrapS));
		glTextureParameteri(glTextureId, GL_TEXTURE_WRAP_T, OpenGLBackend::toGLEnum(textureCreateInfo.samplerSetting.wrapT));
		//glTextureParameterf(glTextureId, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);

		*/

		bool hasMipmaps = OpenGLBackend::requiresMipmaps(textureCreateInfo.samplerSetting.minFilter);
		//int levels = hasMipmaps ? static_cast<int>(std::floor(std::log2(std::max(textureCreateInfo.width, textureCreateInfo.height)))) + 1 : 1;
		int levels = 1;
		if (hasMipmaps)
		{
			levels = 0;
			int s = std::max(textureCreateInfo.width, textureCreateInfo.height);
			while (s > 0) { s >>= 1; levels++; }
		}

		glTextureStorage2D(glTextureId, levels, glInternalFormat, textureCreateInfo.width, textureCreateInfo.height);


		if (textureCreateInfo.data != nullptr)
		{
			glTextureSubImage2D(
				glTextureId,
				0,              // Level 0
				0, 0,           // Offset X, Y
				textureCreateInfo.width,
				textureCreateInfo.height,
				glSourceFormat,
				glSourceType,
				textureCreateInfo.data
			);
		}




		if (hasMipmaps)
		{
			glGenerateTextureMipmap(glTextureId);
		}


		//Not supported by renderDoc!!
		GLuint64 residentHandle = glGetTextureSamplerHandleARB(glTextureId, samplerId);
		
		assert(residentHandle != 0);
		glMakeTextureHandleResidentARB(residentHandle);

		
	


		//residentHandle = glGetTextureHandleARB(glTextureId);
		//glMakeTextureHandleResidentARB(residentHandle);

	
	//m_allotedGLTextureIDs.push_back(glTextureId); // For destruction tracking




		TextureInfo info;
		
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



		GLenum glInternalFormat = OpenGLBackend::toGLEnum(bufferTextureCreateInfo.textureInternalFormat);

		
		

		GLenum target = OpenGLBackend::toGLEnum(bufferTextureCreateInfo.type);

	


		GLuint glTextureId;
		glCreateTextures(target, 1, &glTextureId);
		glBindTexture(target, glTextureId);


	
		glTexStorage2D(
			GL_TEXTURE_2D,
			1, 
			glInternalFormat,
			bufferTextureCreateInfo.width, 
			bufferTextureCreateInfo.height
		);





		GLuint64 residentHandle = 0;


		//residentHandle = glGetTextureHandleARB(glTextureId);

		//assert(residentHandle != 0);
		//glMakeTextureHandleResidentARB(residentHandle);

		//std::cout<<GLErrorToString(glGetError())<<std::endl;


	//m_allotedGLTextureIDs.push_back(glTextureId); // For destruction tracking




		TextureInfo info;
		//info.textureHandle = glTextureId;
		info.resisdentHandle = residentHandle;
		info.type = bufferTextureCreateInfo.type;
		info.width = bufferTextureCreateInfo.width;
		info.height = bufferTextureCreateInfo.height;
		info.internalFormat = bufferTextureCreateInfo.textureInternalFormat;
		//info.samplerSettings = samplerSettings;
		info.hasMipmaps = false;


		m_residentHandleToTextureApiHandle.emplace(residentHandle, glTextureId);// info);
		m_totalAllocatedTextureMemory += bufferTextureCreateInfo.width * bufferTextureCreateInfo.height * OpenGLBackend::getBytesPerPixel(bufferTextureCreateInfo.textureInternalFormat);

		return info;
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