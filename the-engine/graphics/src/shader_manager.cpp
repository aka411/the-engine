#include "shader_manager.h"
#include <stdexcept>
#include <utlis.h>







shaderc_shader_kind ShaderManager::getShadercKind(ShaderType shaderType)
{
	switch (shaderType)
	{
	case ShaderType::VERTEX:
		return shaderc_vertex_shader;
	case ShaderType::FRAGMENT:
		return shaderc_fragment_shader;
	case ShaderType::COMPUTE:
		return shaderc_compute_shader;

	default:
		return shaderc_glsl_infer_from_source;
	}



	
}

ShaderManager::ShaderManager(VulkanDevice& vulkanDevice) : m_vulkanDevice(vulkanDevice)
{
	//need to check version
	m_compileOptions.SetTargetSpirv(shaderc_spirv_version_1_6);

	m_compileOptions.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);

	// You may also need to explicitly enable the capability in shaderc
	// if setting the target environment isn't enough:



	m_shaderProgramMVP.vertexShader = CreateShaderModule(m_vertexShaderCode.c_str(), ShaderType::VERTEX);
	m_shaderProgramMVP.fragmentShader = CreateShaderModule(m_fragmentShaderCode.c_str(), ShaderType::FRAGMENT);



		
}

ShaderManager::~ShaderManager()
{

	for (auto shaderModule : m_shaderModules)
	{
		vkDestroyShaderModule(m_vulkanDevice.getLogicalDeviceHandle(), shaderModule, nullptr);
	}
}

VkShaderModule ShaderManager::CreateShaderModule(const std::string& codeString, ShaderType shadertype)
{

	//convert text to spirv
	shaderc_shader_kind shaderKind = getShadercKind(shadertype);
	
	shaderc::SpvCompilationResult module = m_SprivCompiler.CompileGlslToSpv(codeString, shaderKind, "shader", m_compileOptions);

	if (module.GetCompilationStatus() != shaderc_compilation_status_success)
	{
		std::cout << shaderKind << std::endl;
		std::cout << module.GetErrorMessage()<<std::endl;
		throw std::runtime_error("failed to compile shader: " + module.GetErrorMessage());
	}
	std::vector<uint32_t> spirvCode(module.cbegin(), module.cend());

	// create shader module
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = spirvCode.size() * sizeof(uint32_t);
	createInfo.pCode = reinterpret_cast<const uint32_t*>(spirvCode.data());

	VkShaderModule shaderModule;

	if (vkCreateShaderModule(m_vulkanDevice.getLogicalDeviceHandle(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create shader module!");

	}


	m_shaderModules.push_back(shaderModule);

	return shaderModule;

}

ShaderProgram ShaderManager::getShaderProgram()
{


	return m_shaderProgramMVP;
}
