
#include <cassert>
#include "glad/glad.h"
#include "rendering-system/low-level-gpu-systems/gpu_texture_manager.h"
#include "rendering-system/api-backend/opengl/opengl_texture_helper.h"
#include <rendering-system/low-level-gpu-systems/gpu_sampler_manager.h>

//need code from old code base


namespace  TheEngine::RenderingSystem
{




	GPUTextureManager::GPUTextureManager(GPUSamplerManager& gpuSamplerManager):
		m_gpuSamplerManager(gpuSamplerManager)
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


		GPUSampler gpuSampler =  m_gpuSamplerManager.getOrCreateGPUSampler(samplerSettings);
		GLuint samplerId = static_cast<GLuint>(gpuSampler.residentHandle);



		GLuint glTextureId;
		glCreateTextures(GL_TEXTURE_2D, 1, &glTextureId);
	


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


		if (textureCreateInfo.memoryBlock.getData() != nullptr)
		{
			glTextureSubImage2D(
				glTextureId,
				0,              // Level 0
				0, 0,           // Offset X, Y
				textureCreateInfo.width,
				textureCreateInfo.height,
				glSourceFormat,
				glSourceType,
				textureCreateInfo.memoryBlock.getData()
			);
		}




		if (hasMipmaps)
		{
			glGenerateTextureMipmap(glTextureId);
		}



		GLuint64 residentHandle = glGetTextureSamplerHandleARB(glTextureId, samplerId);
		assert(residentHandle != 0);
		glMakeTextureHandleResidentARB(residentHandle);


	


		//residentHandle = glGetTextureHandleARB(glTextureId);
		//glMakeTextureHandleResidentARB(residentHandle);
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


	
	TextureInfo  GPUTextureManager::createRenderTarget(const RenderTargetCreateInfo& renderTargetCreateInfo)
	{



		GLenum glInternalFormat = OpenGLBackend::toGLEnum(renderTargetCreateInfo.internalFormat);

		
		

		GLenum target = OpenGLBackend::toGLEnum(renderTargetCreateInfo.type);

		//SamplerSetting samplerSettings = bufferTextureCreateInfo.


		GLuint glTextureId;
		glCreateTextures(target, 1, &glTextureId);
		glBindTexture(target, glTextureId);


	
		glTexStorage2D(
			GL_TEXTURE_2D,
			1, 
			glInternalFormat,
			renderTargetCreateInfo.width,
			renderTargetCreateInfo.height
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
		info.type = renderTargetCreateInfo.type;
		info.width = renderTargetCreateInfo.width;
		info.height = renderTargetCreateInfo.height;
		info.internalFormat = renderTargetCreateInfo.internalFormat;
		//info.samplerSettings = samplerSettings;
		info.hasMipmaps = false;


		m_residentHandleToTextureApiHandle.emplace(residentHandle, glTextureId);// info);
		m_totalAllocatedTextureMemory += renderTargetCreateInfo.width * renderTargetCreateInfo.height * OpenGLBackend::getBytesPerPixel(renderTargetCreateInfo.internalFormat);

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