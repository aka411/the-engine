
#include <string.h>
#include <utlis.h>
#include <vulkan_device.h>
#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS  0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS   0
#include "vk_mem_alloc.h"



VulkanDevice::VulkanDevice(vkb::Instance instance, VkSurfaceKHR surface) 
{

	


	m_instance = instance;
	m_surface = surface;

	//select physical device
	vkb::PhysicalDeviceSelector physicalDeviceSelector{ m_instance, m_surface };

	// Enable base features (e.g., sampler anisotropy)
	VkPhysicalDeviceFeatures enabled_features{};
	enabled_features.samplerAnisotropy = VK_TRUE;

	VkPhysicalDeviceFeatures required_features{};
	required_features.shaderInt64 = VK_TRUE;

	physicalDeviceSelector.set_minimum_version(1, 3);
	physicalDeviceSelector.prefer_gpu_device_type(vkb::PreferredDeviceType::discrete);
	physicalDeviceSelector.allow_any_gpu_device_type(true);
	physicalDeviceSelector.require_present(true);
	physicalDeviceSelector.set_surface(m_surface);
	physicalDeviceSelector.add_required_extension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	physicalDeviceSelector.add_required_extension(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
	physicalDeviceSelector.add_required_extension(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME);
	
	physicalDeviceSelector.set_required_features(enabled_features);
	physicalDeviceSelector.set_required_features(required_features);

	auto physicalDeviceSelectionResult = physicalDeviceSelector.select();
	if (!physicalDeviceSelectionResult)
	{
		std::string errorMessage = "Failed to select physical device: ";
		errorMessage += physicalDeviceSelectionResult.error().message();

		logger(errorMessage.c_str()); // Assuming logger takes const char*
		return;
	}
















	m_physicalDevice = physicalDeviceSelectionResult.value();
	//create logical device

	vkb::DeviceBuilder deviceBuilder{ physicalDeviceSelectionResult.value() };

	VkPhysicalDeviceBufferDeviceAddressFeatures BufferDeviceAddressFeatures{};
	BufferDeviceAddressFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES;
	BufferDeviceAddressFeatures.bufferDeviceAddress = VK_TRUE;
	BufferDeviceAddressFeatures.bufferDeviceAddressCaptureReplay = VK_TRUE;

	VkPhysicalDeviceScalarBlockLayoutFeatures scalarBlockLayoutFeatures = {};
	scalarBlockLayoutFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SCALAR_BLOCK_LAYOUT_FEATURES;
	scalarBlockLayoutFeatures.scalarBlockLayout = VK_TRUE; 


	VkPhysicalDeviceDynamicRenderingFeatures dynamicRenderingFeatures{};
	dynamicRenderingFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES;
	dynamicRenderingFeatures.dynamicRendering = VK_TRUE;

	auto logicalDeviceResult = deviceBuilder
		.add_pNext(&dynamicRenderingFeatures)
		.add_pNext(&BufferDeviceAddressFeatures)
		.add_pNext(&scalarBlockLayoutFeatures)
		.build();

	if (!logicalDeviceResult)
	{
		logger("Failed to create logical device: " /** + logicalDeviceResult.error().message().c_str()*/);
		return;
	}

	// Store the final device object
	vkb::Device vkb_device = logicalDeviceResult.value();
	VkDevice device = vkb_device.device;
	m_logicalDevice = logicalDeviceResult.value();
	// You can now retrieve your separate queues:


	volkLoadDevice(m_logicalDevice);

	volkLoadDeviceTable(&m_volkDeviceTable, m_logicalDevice);


	//Need to use volk to load table and pass it to vma 
	VmaVulkanFunctions vmaVulkanFunctions{};

	// VMA allocator creation

	VmaAllocatorCreateInfo allocatorCreateInfo = {};
	allocatorCreateInfo.flags = VMA_ALLOCATOR_CREATE_EXT_MEMORY_BUDGET_BIT | VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
	allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_3;
	allocatorCreateInfo.physicalDevice = m_physicalDevice;
	allocatorCreateInfo.device = m_logicalDevice;
	allocatorCreateInfo.instance = m_instance;



	vmaImportVulkanFunctionsFromVolk(&allocatorCreateInfo, &vmaVulkanFunctions);

	allocatorCreateInfo.pVulkanFunctions = &vmaVulkanFunctions;

	vmaCreateAllocator(&allocatorCreateInfo, &m_vmaAllocator);





}

VulkanDevice::~VulkanDevice()
{
}
