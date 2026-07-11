#include <rendering-system/api-backend/vulkan/utils/vulkan_shader_compiler.h>
#include <iostream>
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_context.h>
#include <rendering-system/api-backend/vulkan/utils/shader_reflection_helper.h>


namespace TheEngine::RenderingSystem::VulkanBackend
{


	shaderc_shader_kind getShadercKind(const ShaderType& shadertype)
	{
		switch (shadertype)
		{
		case ShaderType::VERTEX:                  return shaderc_glsl_vertex_shader;
		case ShaderType::FRAGMENT:                return shaderc_glsl_fragment_shader;
		case ShaderType::COMPUTE:                 return shaderc_glsl_compute_shader;
		case ShaderType::GEOMETRY:                return shaderc_glsl_geometry_shader;
		case ShaderType::TESSELLATION_CONTROL:    return shaderc_glsl_tess_control_shader;
		case ShaderType::TESSELLATION_EVALUATION: return shaderc_glsl_tess_evaluation_shader;
		default:
			throw std::runtime_error("Unknown shader type encountered!");
		}
	}




	VulkanShaderCompiler::VulkanShaderCompiler()
	{



		m_compileOptions.SetTargetSpirv(shaderc_spirv_version_1_6);

		m_compileOptions.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);


	}



	std::vector<uint32_t> VulkanShaderCompiler::compileStringToSpirv(const std::string& codeString, const ShaderType& shadertype)
	{



		const shaderc_shader_kind shaderKind = getShadercKind(shadertype);

		const shaderc::SpvCompilationResult module = m_SpirvCompiler.CompileGlslToSpv(codeString, shaderKind, "shader", m_compileOptions);

		if (module.GetCompilationStatus() != shaderc_compilation_status_success)
		{
			std::cout << shaderKind << std::endl;
			std::cout << module.GetErrorMessage() << std::endl;

			throw std::runtime_error("failed to compile shader: " + module.GetErrorMessage());
		}



		std::vector<uint32_t> spirvCode(module.cbegin(), module.cend());

		return spirvCode;

	}






}