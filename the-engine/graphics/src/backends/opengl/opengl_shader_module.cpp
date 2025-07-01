#include "opengl_shader_module.h"
namespace TheEngine::Graphics
{
	OpenGLShaderModule::~OpenGLShaderModule()
	{
		if (m_shaderId != 0)
		{
			glDeleteShader(m_shaderId);
		}
	}

	ShaderStage OpenGLShaderModule::GetStage() const
	{
		return m_stage;
	}

	const std::string& OpenGLShaderModule::GetEntryPoint() const
	{
		return m_entryPoint;
	}

	GLuint OpenGLShaderModule::GetNativeHandle() const
	{
		 return m_shaderId; 
	}


}