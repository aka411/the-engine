#include <vulkan_device.h>
#include <shader_manager.h>
#include <descriptor_set_manager.h>


class PipelineManager
{
private:
	VulkanDevice& m_vulkanDevice;
	ShaderManager m_shaderManager;
	DescriptorSetManager& m_descriptorSetManager; // passed in 

	VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;
	VkPipelineLayout m_pipeLineLayout = VK_NULL_HANDLE;
	std::vector<VkDescriptorSetLayout>& getDescriptorSetLayout();
	
	void createPipelineLayout();

public:

	PipelineManager(VulkanDevice& vulkanDevice, DescriptorSetManager& descriptorSetManager);

	VkPipeline& getPipeline();
	VkPipelineLayout& getPipelineLayout();
};