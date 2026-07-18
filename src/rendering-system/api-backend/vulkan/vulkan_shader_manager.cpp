#include <iostream>

#include <rendering-system/api-backend/vulkan/vulkan_shader_manager.h>
#include <rendering-system/api-backend/vulkan/utils/shader_reflection_helper.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{


	VkShaderModule VulkanShaderManager::createVkShaderModule(std::vector<uint32_t>& spirvCode, const ShaderType shaderType)
	{


		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = spirvCode.size() * sizeof(uint32_t);
		createInfo.pCode = reinterpret_cast<const uint32_t*>(spirvCode.data());

		VkShaderModule shaderModule;

		if (vkCreateShaderModule(m_vulkanContext.vkDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create shader module!");

		}


		return shaderModule;
	}



	VulkanShaderManager::VulkanShaderManager(VulkanContext& vulkanContext) :
		m_vulkanContext(vulkanContext)
	{

	}





	VulkanShaderManager::~VulkanShaderManager()
	{

		for (auto shaderModule : m_vulkanShaders)
		{

			vkDestroyShaderModule(m_vulkanContext.vkDevice, shaderModule.vertexShader, nullptr);
			vkDestroyShaderModule(m_vulkanContext.vkDevice, shaderModule.fragmentShader, nullptr);
		}

	}




	const ShaderHandle VulkanShaderManager::createShader(ShaderCreateInfo shaderCreateInfo)
	{



		VulkanShader vulkanShader;
		ShaderReflection shaderReflection;

		std::map<uint32_t, DescriptorSetLayout> layoutMap;

		for (auto& shaderSourceCode : shaderCreateInfo.shaderSourceCodes)
		{
			std::vector<uint32_t> spirvCode = m_vulkanShaderCompiler.compileStringToSpirv(shaderSourceCode.second, shaderSourceCode.first);


			VkShaderModule vkShaderModule = createVkShaderModule(spirvCode, shaderSourceCode.first);

			reflectDescriptors(layoutMap, spirvCode, shaderSourceCode.first);

			if (shaderSourceCode.first == ShaderType::VERTEX)
			{
				reflectVertexInputs(spirvCode, shaderReflection.getvertexLayout());
				vulkanShader.vertexShader = vkShaderModule;

			}

			if (shaderSourceCode.first == ShaderType::FRAGMENT)
			{
				vulkanShader.fragmentShader = vkShaderModule;

			}

		}


		uint32_t maxSetIndex = 0;
		if (!layoutMap.empty())
		{
			maxSetIndex = layoutMap.rbegin()->first;
		}


		std::vector<DescriptorSetLayout>& descriptorSetLayouts = shaderReflection.getDescriptorSetLayouts();

		descriptorSetLayouts.resize(maxSetIndex + 1);


		for (auto const& [index, layout] : layoutMap)
		{
			descriptorSetLayouts[index] = layout;
		}



		vulkanShader.shaderReflection = shaderReflection;
		ShaderHandle  ShaderHandle{ m_vulkanShaders.size() };
		m_shaderNameToVulkanShader["default"] = ShaderHandle;

		m_vulkanShaders.push_back(vulkanShader);

		return ShaderHandle;
	}



	VulkanShader VulkanShaderManager::getVulkanShader(const ShaderHandle& shaderHandle) const
	{

		if (m_vulkanShaders.size() > shaderHandle.id)
		{
			return m_vulkanShaders[shaderHandle.id];
		}

		assert(false && "Invalid ShaderHandle");

		return VulkanShader{};
	}


}