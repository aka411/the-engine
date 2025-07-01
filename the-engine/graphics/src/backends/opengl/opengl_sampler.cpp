#include "opengl_sampler.h"
namespace TheEngine::Graphics
{


	OpenGLSampler::OpenGLSampler(const GLuint samplerId, const SamplerCreateInfo& samplerCreateInfo)
		: m_samplerCreateInfo(samplerCreateInfo), m_samplerId(samplerId)
	{
	}



OpenGLSampler::~OpenGLSampler()
{
}

GLuint OpenGLSampler::GetNativeHandle() const
{
	return m_samplerId;
}

}
