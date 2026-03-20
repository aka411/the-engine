#include <rendering-system/low-level-gpu-systems/gpu_sampler_manager.h>
#include <rendering-system/api-backend/opengl/opengl_texture_helper.h>
#include <glad/glad.h>

namespace TheEngine::RenderingSystem
{



	GPUSamplerManager::GPUSamplerManager()
	{
		//just for safety
		static_assert(sizeof(GLuint) <= sizeof(uint64_t), "GPUSampler: GLuint is larger than the storage type uint64_t; handle truncation will occur.");

	}





	GPUSampler GPUSamplerManager::getOrCreateGPUSampler(const SamplerSetting& samplerSetting)
	{

		auto it = m_cachedGPUSamplers.find(samplerSetting);
		if (it != m_cachedGPUSamplers.end()) {
			return  it->second;
		}


		GLuint samplerId;
		glCreateSamplers(1, &samplerId);

		glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, OpenGLBackend::toGLEnum(samplerSetting.minFilter));
		glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, OpenGLBackend::toGLEnum(samplerSetting.magFilter));
		glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_S, OpenGLBackend::toGLEnum(samplerSetting.wrapS));
		glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_T, OpenGLBackend::toGLEnum(samplerSetting.wrapT));

		//yeah this is like holding gun to my foot
        //uint64_t should be large enough to hold GLuint
        //static_cast should correctly put the lower bytes in lower part of uint64_t
        //Also need to cast to GLuint where ever we use this 
        //trust me, bro i might know what i am doing
		GPUSampler gpuSampler{ static_cast<uint64_t>(samplerId) };
		m_cachedGPUSamplers[samplerSetting] = gpuSampler;



		return  gpuSampler;

		}


			GPUSamplerManager::~GPUSamplerManager()
		{

			for (const auto& [settings, gpuSampler] : m_cachedGPUSamplers)
			{
				const GLuint openglHandle = static_cast<GLuint>(gpuSampler.residentHandle);
				glDeleteSamplers(1, &openglHandle);
			}

		}

	}