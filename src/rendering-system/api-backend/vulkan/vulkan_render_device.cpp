#include <rendering-system/api-backend/vulkan/vulkan_render_device.h>

#include <rendering-system/api-backend/vulkan/vulkan_buffer_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_pipeline_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_sampler_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_texture_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_shader_manager.h>

#include <rendering-system/api-backend/vulkan/vulkan_transfer_manager.h>

#include <rendering-system/api-backend/vulkan/vulkan_presentation_system.h>
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_descriptor_set_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_command_buffer_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_resource_resolver.h>

#include <VkBootstrap.h>
#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS  0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS   0
#include <vk_mem_alloc.h>

#include <iostream>


namespace TheEngine::RenderingSystem::VulkanBackend
{

	VulkanRenderDevice::VulkanRenderDevice(VkSurfaceKHR surface, vkb::Instance instance) :
		IRenderDevice(RenderingAPI::VULKAN_1_3)
		
	{



		m_instance = instance;
		m_surface = surface;

	
		vkb::PhysicalDeviceSelector physicalDeviceSelector{ m_instance, m_surface };



		physicalDeviceSelector.set_minimum_version(1, 3);
		physicalDeviceSelector.prefer_gpu_device_type(vkb::PreferredDeviceType::discrete);
		physicalDeviceSelector.allow_any_gpu_device_type(true);
		physicalDeviceSelector.require_present(true);
		physicalDeviceSelector.set_surface(m_surface);

		physicalDeviceSelector.add_required_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		physicalDeviceSelector.add_required_extension(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
		physicalDeviceSelector.add_required_extension(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME);
		physicalDeviceSelector.add_required_extension(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME);



		auto physicalDeviceSelectionResult = physicalDeviceSelector.select();
		if (!physicalDeviceSelectionResult)
		{
			std::string errorMessage = "Failed to select physical device: ";
			errorMessage += physicalDeviceSelectionResult.error().message();

			std::cout << errorMessage.c_str() << "\n";

			assert(false && errorMessage.c_str());
			return;
		}






	
		VkPhysicalDeviceDescriptorIndexingFeatures indexingFeatures{};
		indexingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;

	
		indexingFeatures.descriptorBindingSampledImageUpdateAfterBind = VK_TRUE;
		indexingFeatures.descriptorBindingPartiallyBound = VK_TRUE;
		indexingFeatures.descriptorBindingVariableDescriptorCount = VK_TRUE;

		indexingFeatures.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
		indexingFeatures.shaderStorageBufferArrayNonUniformIndexing = VK_TRUE;
		indexingFeatures.runtimeDescriptorArray = VK_TRUE;





		m_physicalDevice = physicalDeviceSelectionResult.value();


		//Logical Device


		vkb::DeviceBuilder deviceBuilder{ physicalDeviceSelectionResult.value() };

		VkPhysicalDeviceBufferDeviceAddressFeatures bufferDeviceAddressFeatures{};
		bufferDeviceAddressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
		bufferDeviceAddressFeatures.bufferDeviceAddress = VK_TRUE;
		//bufferDeviceAddressFeatures.bufferDeviceAddressCaptureReplay = VK_TRUE; //NOTE: Enable later when debugging

		//Disabled for now
		//VkPhysicalDeviceScalarBlockLayoutFeatures scalarBlockLayoutFeatures = {};
		//scalarBlockLayoutFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES;
		//scalarBlockLayoutFeatures.scalarBlockLayout = VK_TRUE;

		VkPhysicalDeviceDynamicRenderingFeatures dynamicRenderingFeatures{};
		dynamicRenderingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES;
		dynamicRenderingFeatures.dynamicRendering = VK_TRUE;

		VkPhysicalDeviceSynchronization2Features sync2Features{};
		sync2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES;
		sync2Features.synchronization2 = VK_TRUE;
		


		auto logicalDeviceResult = deviceBuilder
			.add_pNext(&dynamicRenderingFeatures)
			.add_pNext(&bufferDeviceAddressFeatures)
			//.add_pNext(&scalarBlockLayoutFeatures)
			.add_pNext(&indexingFeatures)
			.add_pNext(&sync2Features)
			.build();

		if (!logicalDeviceResult)
		{
			//I am for now gonna enforce a strict requirements for MVP
			assert(false && ("Failed to create logical device : " + logicalDeviceResult.error().message()).c_str());
			return;
		}


		vkb::Device vkb_device = logicalDeviceResult.value();
		VkDevice device = vkb_device.device;
		m_logicalDevice = logicalDeviceResult.value();



		volkLoadDevice(m_logicalDevice);





		// VMA allocator creation

		VmaAllocatorCreateInfo allocatorCreateInfo = {};
		allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT | VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
		allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
		allocatorCreateInfo.physicalDevice = m_physicalDevice;
		allocatorCreateInfo.device = m_logicalDevice;
		allocatorCreateInfo.instance = m_instance;


	
		VmaVulkanFunctions vmaVulkanFunctions{};
		vmaImportVulkanFunctionsFromVolk(&allocatorCreateInfo, &vmaVulkanFunctions);

		allocatorCreateInfo.pVulkanFunctions = &vmaVulkanFunctions;

		vmaCreateAllocator(&allocatorCreateInfo, &m_vmaAllocator);





		m_vulkanContext.vkbDevice = m_logicalDevice;

		m_vulkanContext.vkDevice = m_logicalDevice.device;
		m_vulkanContext.vkInstance = m_instance.instance;
		m_vulkanContext.vkPhysicalDevice = m_physicalDevice.physical_device;
		m_vulkanContext.vkSurfaceKHR = m_surface;
		m_vulkanContext.vmaAllocator = m_vmaAllocator;


		m_samplerManager = std::make_unique<VulkanSamplerManager>(m_vulkanContext);
		m_vulkanTextureStore = std::make_unique<VulkanTextureStore>(m_vulkanContext,static_cast<VulkanSamplerManager&>(*m_samplerManager));
	


		m_vulkanQueueManager = std::make_unique<VulkanQueueManager>(m_vulkanContext);
		m_vulkanDescriptorSetManager = std::make_unique<VulkanDescriptorSetManager>(m_vulkanContext);

		m_shaderManager = std::make_unique<VulkanShaderManager>(m_vulkanContext);
		m_bufferManager = std::make_unique<VulkanBufferManager>(m_vmaAllocator);

		m_vulkanSwapchainManager = std::make_unique<VulkanSwapchainManager>(m_vulkanContext);
		m_pipelineManager = std::make_unique<VulkanPipelineManager>(m_vulkanContext, *m_vulkanDescriptorSetManager, static_cast<VulkanShaderManager&>(*m_shaderManager));
		m_vulkanResourceResolver = std::make_unique<VulkanResourceResolver>(*m_vulkanSwapchainManager,*m_vulkanTextureStore, static_cast<VulkanBufferManager&>(*m_bufferManager), static_cast<VulkanPipelineManager&>(*m_pipelineManager));


		m_vulkanCommandBufferManager = std::make_unique<VulkanCommandBufferManager>(m_vulkanContext, *m_vulkanQueueManager, *m_vulkanResourceResolver,*m_vulkanDescriptorSetManager);
		m_presentationSystem = std::make_unique<VulkanPresentationSystem>(m_vulkanContext, *m_vulkanSwapchainManager, *m_vulkanCommandBufferManager);



		m_transferManager = std::make_unique<VulkanTransferManager>(m_vulkanContext, *m_vulkanCommandBufferManager, static_cast<VulkanBufferManager&>(*m_bufferManager));
		m_textureManager = std::make_unique<VulkanTextureManager>(*m_vulkanTextureStore, static_cast<VulkanTransferManager&>(*m_transferManager));

	}


	VulkanRenderDevice::~VulkanRenderDevice()
	{
		//TODO : cleanup
	}


	void VulkanRenderDevice::beginFrame(const uint32_t frameIndex)
	{
		m_vulkanDescriptorSetManager->setCurrentFrame(frameIndex);

	}
	

}