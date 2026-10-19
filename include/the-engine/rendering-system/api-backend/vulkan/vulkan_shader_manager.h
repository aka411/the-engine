#pragma once
#include <vector>

#include <volk.h>

#include <rendering-system/rhi/i_shader_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_render_device.h>
#include <rendering-system/rhi/data-structures/vertex_layout.h>
#include <rendering-system/rhi/data-structures/descriptor_set_layout.h>
#include <rendering-system/api-backend/vulkan/utils/vulkan_shader_compiler.h>


namespace TheEngine::Platform
{
	class FileSystem;

}

namespace TheEngine::RenderingSystem::VulkanBackend
{


	class ShaderReflection
	{
	private:

		

		std::vector<DescriptorSetLayout> m_descriptorSetLayouts;

		VertexLayout m_vertexLayout;

	public:


		std::vector<DescriptorSetLayout>& getDescriptorSetLayouts()
		{
			return m_descriptorSetLayouts;
		}



		VertexLayout& getvertexLayout()
		{
			return m_vertexLayout;
		}

	};





	struct VulkanShader
	{
		ShaderReflection shaderReflection;
		VkShaderModule vertexShader = VK_NULL_HANDLE;
		VkShaderModule fragmentShader = VK_NULL_HANDLE;

	};


	class VulkanShaderManager : public IShaderManager
	{

	private:

		VulkanContext& m_vulkanContext;

		VulkanShaderCompiler m_vulkanShaderCompiler;

		std::vector<VulkanShader> m_vulkanShaders;
		std::map<std::string, ShaderHandle> m_shaderNameToVulkanShader;



		VkShaderModule createVkShaderModule(std::vector<uint32_t>& spirvCode, const ShaderType shaderType);

	public:

		VulkanShaderManager(VulkanContext& vulkanContext);

		virtual ~VulkanShaderManager() override;


		virtual const ShaderHandle createShader(ShaderCreateInfo shaderCreateInfo) override;
		//Getters :
		ShaderHandle getShaderHandle(const std::string& shaderName);


		//Vulkan Specific Methods
		VulkanShader getVulkanShader(const ShaderHandle& shaderHandle) const;
		VulkanShader getVulkanShader(const std::string& shaderName) const;


	};



}