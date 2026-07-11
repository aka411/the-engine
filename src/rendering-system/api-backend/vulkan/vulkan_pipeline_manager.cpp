#include <volk.h>

#include <rendering-system/api-backend/vulkan/vulkan_pipeline_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_shader_manager.h>
#include <rendering-system/api-backend/vulkan/utils/engine_to_vulkan_pipeline_helpers.h>
#include <rendering-system/api-backend/vulkan/utils/engine_to_vulkan_types.h>



namespace TheEngine::RenderingSystem::VulkanBackend
{


	PipelineHandle VulkanPipelineManager::storeVulkanPipelineObject(VkPipeline vkPipeline, VkPipelineLayout vkPipelineLayout, std::span<VkDescriptorSetLayout> vkDescriptorSetLayouts)
	{

		VulkanPipelineObject vulkanPipelineObject;
		PipelineHandle pipelineHandle;
		pipelineHandle.id = m_vulkanPipelineObjects.size();

		vulkanPipelineObject.m_vkPipeline = vkPipeline;
		vulkanPipelineObject.m_vkPipelineLayout = vkPipelineLayout;
		vulkanPipelineObject.m_pushConstantStages = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

		vulkanPipelineObject.m_vkDescriptorSetLayouts.reserve(vkDescriptorSetLayouts.size());

		for (int i = 0 ; i < vkDescriptorSetLayouts.size(); ++i)
		{
			vulkanPipelineObject.m_vkDescriptorSetLayouts.push_back(vkDescriptorSetLayouts[i]);
		}

		m_vulkanPipelineObjects.push_back(vulkanPipelineObject);

		return pipelineHandle;

	}



	VulkanPipelineManager::VulkanPipelineManager(VulkanContext& vulkanContext, VulkanDescriptorSetManager& vulkanDescriptorSetManager, VulkanShaderManager& vulkanShaderManager) :
		m_vulkanContext(vulkanContext),
		m_vulkanDescriptorSetManager(vulkanDescriptorSetManager),
		m_vulkanShaderManager(vulkanShaderManager)
	{



	}


	VulkanPipelineManager::~VulkanPipelineManager()
	{


	}


	VkPipelineLayout VulkanPipelineManager::createVkPipelineLayout(VkPipelineLayoutCreateInfo& VkPipelineLayoutCreateInfo)
	{
		VkPipelineLayout vkPipelineLayout;

		if (vkCreatePipelineLayout(m_vulkanContext.vkDevice, &VkPipelineLayoutCreateInfo, nullptr, &vkPipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout!");
		}

		return vkPipelineLayout;

	}
	
	const PipelineHandle VulkanPipelineManager::createVulkanPipelineObject(const std::string& pipelineName, const VkGraphicsPipelineCreateInfo& vkGraphicsPipelineCreateInfo, const VkPipelineLayout vkPipelineLayout, const std::span<VkDescriptorSetLayout> vkDescriptorSetLayouts)
	{

		VkPipeline vkPipeline;

		if (vkCreateGraphicsPipelines(m_vulkanContext.vkDevice, VK_NULL_HANDLE, 1, &vkGraphicsPipelineCreateInfo, nullptr, &vkPipeline) != VK_SUCCESS)
		{
			assert(false && "failed to create graphics pipeline!");
		}

		PipelineHandle pipelineHandle = storeVulkanPipelineObject(vkPipeline, vkPipelineLayout, vkDescriptorSetLayouts);

		m_stringNameToPipelineHandle[pipelineName] = pipelineHandle;

	

		return pipelineHandle;
	}
	
	const PipelineHandle VulkanPipelineManager::createPipeline(const std::string& name, const PipelineStateCreateInfo& pipelineStateCreateInfo)
	{

		


		// LOCAL DATA STORAGE

			std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
			std::vector<VkVertexInputBindingDescription> vertexBindings;
			std::vector<VkVertexInputAttributeDescription> vertexAttributes;
			std::vector<VkFormat> colorFormats;
			std::vector<VkPipelineColorBlendAttachmentState> blendAttachments;
			std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

			//SHADER STAGES
			auto [reflection, vMod, fMod] = m_vulkanShaderManager.getVulkanShader(pipelineStateCreateInfo.shaderHandle);

			VkPipelineShaderStageCreateInfo vertStage{};
			vertStage.sType =  VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO ;
			vertStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vertStage.module = vMod;
			vertStage.pName = "main";
			shaderStages.push_back(vertStage);

			VkPipelineShaderStageCreateInfo fragStage{};
			fragStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragStage.module = fMod;
			fragStage.pName = "main";
			shaderStages.push_back(fragStage);


			// VERTEX INPUT
			
			getVkVertexlayout(reflection.getvertexLayout(), vertexBindings, vertexAttributes);

			VkPipelineVertexInputStateCreateInfo vertexInputInfo = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
			vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexBindings.size());
			vertexInputInfo.pVertexBindingDescriptions = vertexBindings.data();
			vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributes.size());
			vertexInputInfo.pVertexAttributeDescriptions = vertexAttributes.data();

			//FIXED FUNCTIONS
			VkPipelineInputAssemblyStateCreateInfo inputAssembly = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

			VkPipelineRasterizationStateCreateInfo rasterizer = getVkPipelineRasterizationStateCreateInfo(pipelineStateCreateInfo.pipelineStateConfig.rasterizerState);
			VkPipelineDepthStencilStateCreateInfo depthStencil = getVkPipelineDepthStencilStateCreateInfo(pipelineStateCreateInfo.pipelineStateConfig.depthStencilState);

			
			VkPipelineMultisampleStateCreateInfo multisampling = getVkPipelineMultisampleStateCreateInfo(pipelineStateCreateInfo.pipelineStateConfig.multisampleState);
	


			//COLOR BLENDING (Dynamic Rendering Needs This)
			assert(pipelineStateCreateInfo.pipelineStateConfig.colorAttachmentBlendStates.size() >= pipelineStateCreateInfo.renderOutputConfiguration.colorAttachments.size());

			for (uint32_t i = 0; i < pipelineStateCreateInfo.renderOutputConfiguration.colorAttachments.size(); ++i)
				//for (auto& color : m_pipelineStateCreateInfo.renderOutputConfiguration.colorAttachments) 
			{
				auto& color = pipelineStateCreateInfo.renderOutputConfiguration.colorAttachments[i];
				auto& blendState = pipelineStateCreateInfo.pipelineStateConfig.colorAttachmentBlendStates[i];

				VkPipelineColorBlendAttachmentState att = {};

	
			
				att.colorWriteMask = static_cast<VkColorComponentFlags>(blendState.colorWriteMask);

			
				att.blendEnable = getVkBool(blendState.blendEnable);


				att.srcColorBlendFactor = getVkBlendFactor(blendState.srcColorBlendFactor);
				att.dstColorBlendFactor = getVkBlendFactor(blendState.dstColorBlendFactor);
				att.colorBlendOp = getVkBlendOp(blendState.colorBlendOp);

				att.srcAlphaBlendFactor = getVkBlendFactor(blendState.srcAlphaBlendFactor);
				att.dstAlphaBlendFactor = getVkBlendFactor(blendState.dstAlphaBlendFactor);
				att.alphaBlendOp = getVkBlendOp(blendState.alphaBlendOp);

				blendAttachments.push_back(att);


				colorFormats.push_back(resourceFormatToVkFormat(color.colorFormat));

			}



			VkPipelineColorBlendStateCreateInfo colorBlending = { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
			colorBlending.attachmentCount = static_cast<uint32_t>(blendAttachments.size());
			colorBlending.pAttachments = blendAttachments.data();




			//VIEWPORT / DYNAMIC STATE
			VkPipelineViewportStateCreateInfo viewportState = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
			viewportState.viewportCount = 1;
			viewportState.scissorCount = 1;

			VkPipelineDynamicStateCreateInfo dynamicInfo = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
			dynamicInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
			dynamicInfo.pDynamicStates = dynamicStates.data();




			//PIPELINE LAYOUT
			auto vkDescriptorSetLayouts = getVkDescriptorSetLayouts(reflection.getDescriptorSetLayouts(),m_vulkanDescriptorSetManager);

			VkPipelineLayoutCreateInfo layoutInfo = { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
			layoutInfo.setLayoutCount = static_cast<uint32_t>(vkDescriptorSetLayouts.size());
			layoutInfo.pSetLayouts = vkDescriptorSetLayouts.data();

			
			VkPushConstantRange pushRange = { VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, 128 };
			layoutInfo.pushConstantRangeCount = 1;
			layoutInfo.pPushConstantRanges = &pushRange;

			VkPipelineLayout vkPipelineLayout = this->createVkPipelineLayout(layoutInfo);



			//DYNAMIC RENDERING
			VkFormat depthFmt = pipelineStateCreateInfo.renderOutputConfiguration.hasDepth ?
				resourceFormatToVkFormat(pipelineStateCreateInfo.renderOutputConfiguration.depthStencilAttachment.colorFormat) : VK_FORMAT_UNDEFINED;

			VkPipelineRenderingCreateInfo renderingInfo = { VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO };
			renderingInfo.colorAttachmentCount = static_cast<uint32_t>(colorFormats.size());
			renderingInfo.pColorAttachmentFormats = colorFormats.data();
			renderingInfo.depthAttachmentFormat = depthFmt;






			// FINAL ASSEMBLY
			VkGraphicsPipelineCreateInfo pipelineInfo{};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.pNext = &renderingInfo;
			pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
			pipelineInfo.pStages = shaderStages.data();
			pipelineInfo.pVertexInputState = &vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &inputAssembly;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &rasterizer;
			pipelineInfo.pMultisampleState = &multisampling;
			pipelineInfo.pDepthStencilState = &depthStencil;
			pipelineInfo.pColorBlendState = &colorBlending;
			pipelineInfo.pDynamicState = &dynamicInfo;
			pipelineInfo.layout = vkPipelineLayout;




			return this->createVulkanPipelineObject(name, pipelineInfo, vkPipelineLayout, vkDescriptorSetLayouts);
	}
	






	void VulkanPipelineManager::destroyPipeline(const PipelineHandle& pipelineHandle)
	{

	}


	//GETTERS

	const PipelineHandle VulkanPipelineManager::getPipelineHandle(const std::string& pipelineName) const 
	{
		return m_stringNameToPipelineHandle.at(pipelineName);
	}


	VulkanPipelineObject VulkanPipelineManager::getVulkanPipelineObject(const std::string& pipelineName) const
	{
		if (m_stringNameToPipelineHandle.find(pipelineName) == m_stringNameToPipelineHandle.end())
		{
			assert(false && ("No Pipeline Named : " + pipelineName).c_str());
		}


		const PipelineHandle pipelineHandle = m_stringNameToPipelineHandle.at(pipelineName);
		return getVulkanPipelineObject(pipelineHandle);
	}


	VulkanPipelineObject VulkanPipelineManager::getVulkanPipelineObject(const PipelineHandle& pipelineHandle) const
	{
		assert(m_vulkanPipelineObjects.size() > pipelineHandle.id && "Inavlid PipelineHandle");


		return m_vulkanPipelineObjects[pipelineHandle.id];


	}

}