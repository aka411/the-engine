#pragma once
#include "../../igpu_sampler.h"


namespace TheEngine::Graphics
{
	class OpenGLSampler : public IGPUSampler
	{
	private:
		const SamplerCreateInfo m_samplerCreateInfo;
		const GLuint m_samplerId;

	public:
		OpenGLSampler(const GLuint samplerId, const SamplerCreateInfo& samplerCreateInfo);
		~OpenGLSampler();

		GLuint GetNativeHandle() const;

	};
}
