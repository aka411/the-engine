#pragma once
#include "../../igpu_shader_module.h"
#include <string>


namespace TheEngine::Graphics
{
	class OpenGLShaderModule : public IGPUShaderModule
	{
	private:
		const GLuint m_shaderId;
		const ShaderStage m_stage;
		const std::string m_entryPoint;

	public:
		OpenGLShaderModule(GLuint shaderId, ShaderStage stage, const std::string& entryPoint)
			: m_shaderId(shaderId), m_stage(stage), m_entryPoint(entryPoint)
		{
		}
		~OpenGLShaderModule();

		ShaderStage GetStage() const override;
		const std::string& GetEntryPoint() const override;

		GLuint GetNativeHandle() const; // For OpenGL specific usage

	};
}