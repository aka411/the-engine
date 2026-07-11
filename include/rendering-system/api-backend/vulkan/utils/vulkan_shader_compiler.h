#pragma once

#include <string.h>
#include <rendering-system/rhi/i_shader_manager.h>
#include <volk.h>
#include <shaderc/shaderc.hpp>


namespace TheEngine::RenderingSystem::VulkanBackend
{

	class VulkanContext;

	class VulkanShaderCompiler
	{
	private:

		shaderc::Compiler m_SpirvCompiler;
		shaderc::CompileOptions m_compileOptions;



	public:

		VulkanShaderCompiler();

		std::vector<uint32_t> compileStringToSpirv(const std::string& codeString, const ShaderType& shadertype);



	};





}