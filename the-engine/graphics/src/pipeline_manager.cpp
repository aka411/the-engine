#include "pipeline_manager.h"
#include "volk.h"
#include <common_data_types.h>
PipelineManager::PipelineManager(VulkanDevice& vulkanDevice, DescriptorSetManager& descriptorSetManager) : m_vulkanDevice(vulkanDevice) , m_shaderManager(vulkanDevice), m_descriptorSetManager(descriptorSetManager)
{


	VkFormat colorFormat = VK_FORMAT_B8G8R8A8_SRGB;

	VkPipelineRenderingCreateInfoKHR pipelineRenderingCreateInfo{};
	pipelineRenderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
	pipelineRenderingCreateInfo.colorAttachmentCount = 1;
	pipelineRenderingCreateInfo.pColorAttachmentFormats = &colorFormat; // Set dynamically
	pipelineRenderingCreateInfo.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT; // Example depth format
	pipelineRenderingCreateInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;





    /**

    typedef struct VkGraphicsPipelineCreateInfo
    {
        VkStructureType                                  sType;
        const void* pNext;
        VkPipelineCreateFlags                            flags;
        uint32_t                                         stageCount;
        const VkPipelineShaderStageCreateInfo* pStages;
        const VkPipelineVertexInputStateCreateInfo* pVertexInputState;
        const VkPipelineInputAssemblyStateCreateInfo* pInputAssemblyState;
        const VkPipelineTessellationStateCreateInfo* pTessellationState;
        const VkPipelineViewportStateCreateInfo* pViewportState;
        const VkPipelineRasterizationStateCreateInfo* pRasterizationState;
        const VkPipelineMultisampleStateCreateInfo* pMultisampleState;
        const VkPipelineDepthStencilStateCreateInfo* pDepthStencilState;
        const VkPipelineColorBlendStateCreateInfo* pColorBlendState;
        const VkPipelineDynamicStateCreateInfo* pDynamicState;
        VkPipelineLayout                                 layout;
        VkRenderPass                                     renderPass;
        uint32_t                                         subpass;
        VkPipeline                                       basePipelineHandle;
        int32_t                                          basePipelineIndex;
    } VkGraphicsPipelineCreateInfo;
    **/

	// 11 structures to define the graphics pipeline


	ShaderProgram shaderProgram = m_shaderManager.getShaderProgram();


	VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo[2]{};
	pipelineShaderStageCreateInfo[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipelineShaderStageCreateInfo[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	pipelineShaderStageCreateInfo[0].module = shaderProgram.vertexShader; // Set your vertex shader module
	pipelineShaderStageCreateInfo[0].pName = "main";

	pipelineShaderStageCreateInfo[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipelineShaderStageCreateInfo[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	pipelineShaderStageCreateInfo[1].module = shaderProgram.fragmentShader; // Set your fragment shader module
	pipelineShaderStageCreateInfo[1].pName = "main";


	VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo{};
	pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = 0; // Set as needed
	pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = nullptr; // Set as needed
	pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = 0; // Set as needed
	pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = nullptr; // Set as needed

	VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo{};
	pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	pipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;
	pipelineInputAssemblyStateCreateInfo.pNext = nullptr;
	pipelineInputAssemblyStateCreateInfo.flags = 0;
	pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;


	VkPipelineTessellationStateCreateInfo pipelineTessellationStateCreateInfo{};
	pipelineTessellationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
	pipelineTessellationStateCreateInfo.pNext = nullptr;
	pipelineTessellationStateCreateInfo.flags = 0;
	pipelineTessellationStateCreateInfo.patchControlPoints = 3; // Example for tessellation, set as needed



	VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo{};
	pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	pipelineViewportStateCreateInfo.pNext = nullptr;
	pipelineViewportStateCreateInfo.viewportCount = 1;
	pipelineViewportStateCreateInfo.pViewports = nullptr;
	pipelineViewportStateCreateInfo.scissorCount = 1;
	pipelineViewportStateCreateInfo.pScissors = nullptr;


	VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo{};
	pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	pipelineRasterizationStateCreateInfo.pNext = nullptr;
	pipelineRasterizationStateCreateInfo.flags = 0;
	pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
	pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	pipelineRasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	pipelineRasterizationStateCreateInfo.lineWidth = 1.0f;
	pipelineRasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	pipelineRasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
	pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f; // Optional
	pipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f; // Optional
	pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f; // Optional



	VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo{};
	pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
	pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;//no multisampling
	pipelineMultisampleStateCreateInfo.minSampleShading = 1.0f; // Optional
	pipelineMultisampleStateCreateInfo.pSampleMask = nullptr; // Optional
	pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE; // Optional
	pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE; // Optional


	VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo{};
	pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	pipelineDepthStencilStateCreateInfo.pNext = nullptr;
	pipelineDepthStencilStateCreateInfo.flags = 0;
	pipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
	pipelineDepthStencilStateCreateInfo.depthTestEnable = VK_FALSE;





	VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState{};
	pipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	pipelineColorBlendAttachmentState.blendEnable = VK_FALSE;
	pipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	pipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	pipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	pipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	pipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	pipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo{};
	pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	pipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
	pipelineColorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
	pipelineColorBlendStateCreateInfo.attachmentCount = 1;
	pipelineColorBlendStateCreateInfo.pAttachments = &pipelineColorBlendAttachmentState;
	pipelineColorBlendStateCreateInfo.blendConstants[0] = 0.0f; // Optional
	pipelineColorBlendStateCreateInfo.blendConstants[1] = 0.0f; // Optional
	pipelineColorBlendStateCreateInfo.blendConstants[2] = 0.0f; // Optional
	pipelineColorBlendStateCreateInfo.blendConstants[3] = 0.0f; // Optional






	std::vector<VkDynamicState> dynamicStates = {
	VK_DYNAMIC_STATE_VIEWPORT,
	VK_DYNAMIC_STATE_SCISSOR
	};




	VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo{};
	pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	pipelineDynamicStateCreateInfo.pNext = nullptr;
	pipelineDynamicStateCreateInfo.flags = 0;
	pipelineDynamicStateCreateInfo.dynamicStateCount = dynamicStates.size(); // Set as needed
	pipelineDynamicStateCreateInfo.pDynamicStates = dynamicStates.data(); // Set as needed
	
	




	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo{};

	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineCreateInfo.pNext = &pipelineRenderingCreateInfo;
	graphicsPipelineCreateInfo.flags = 0;
	graphicsPipelineCreateInfo.stageCount = 2;
	graphicsPipelineCreateInfo.pStages = pipelineShaderStageCreateInfo;
	graphicsPipelineCreateInfo.renderPass = VK_NULL_HANDLE; // Not using a traditional render pass
	graphicsPipelineCreateInfo.subpass = 0;

	graphicsPipelineCreateInfo.pVertexInputState = &pipelineVertexInputStateCreateInfo;
	graphicsPipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
	graphicsPipelineCreateInfo.pTessellationState = nullptr; // Not using tessellation
	graphicsPipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
	graphicsPipelineCreateInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
	graphicsPipelineCreateInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
	graphicsPipelineCreateInfo.pDepthStencilState = &pipelineDepthStencilStateCreateInfo; // Optional
	graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
	graphicsPipelineCreateInfo.pDynamicState = &pipelineDynamicStateCreateInfo;
	createPipelineLayout();//bad design
	graphicsPipelineCreateInfo.layout = getPipelineLayout();

		if (vkCreateGraphicsPipelines(m_vulkanDevice.getLogicalDeviceHandle(), VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create graphics pipeline!");
		}




}

VkPipeline& PipelineManager::getPipeline()
{
	return m_graphicsPipeline;
}





std::vector<VkDescriptorSetLayout>& PipelineManager::getDescriptorSetLayout()
{
	/*
	//call method only once
	// we will have one ssbo for vertex pulling for now
	VkDescriptorSetLayoutBinding globalVertexSSBOLayoutBinding{};
	globalVertexSSBOLayoutBinding.binding = 0;
	globalVertexSSBOLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	globalVertexSSBOLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	globalVertexSSBOLayoutBinding.pImmutableSamplers = nullptr;
	globalVertexSSBOLayoutBinding.descriptorCount = 1;
	

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &globalVertexSSBOLayoutBinding;




	VkDescriptorSetLayout descriptorSetLayout;

	vkCreateDescriptorSetLayout(m_vulkanDevice.getLogicalDeviceHandle(), &layoutInfo, nullptr, &descriptorSetLayout);

	return descriptorSetLayout;*/



	return m_descriptorSetManager.getDescriptorSetLayout();


}

void PipelineManager::createPipelineLayout()
{
	// call only once

//Descriptor set layouts and push constant ranges would go here if used

	std::vector<VkDescriptorSetLayout> descriptorSetLayoutList = getDescriptorSetLayout();


	//TODO : FOR MVP WE WILL NOT USE ANY BINDING OF BUFFER ,WE WILL ONLY USE PUSH CONSTANT !!!!

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = descriptorSetLayoutList.size(); // number of layouts //READ TODO ABOVE
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayoutList.data(); //pointer to layout array // //READ TODO ABOVE


	//Important
	// we need to send a 4x4 matrix and a 64 bit value(BDA address with offset added)
	/*
	* struct pushConst
	* {
	* glm::mat4 modelMatrix;
	* uint64_t ssboOffset = 0;
	*
	* }
	*/

	VkPushConstantRange pushConstantRange;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(PushConst); // change
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;





	if (vkCreatePipelineLayout(m_vulkanDevice.getLogicalDeviceHandle(), &pipelineLayoutInfo, nullptr, &m_pipeLineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}

	
}

VkPipelineLayout& PipelineManager::getPipelineLayout()
{
	return m_pipeLineLayout;
}
