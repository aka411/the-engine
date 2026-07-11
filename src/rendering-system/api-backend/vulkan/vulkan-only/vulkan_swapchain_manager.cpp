#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_swapchain_manager.h>
#include <rendering-system/api-backend/vulkan/vulkan_texture_system_data_types.h>
#include <rendering-system/api-backend/vulkan/vulkan_render_device.h>
#include <rendering-system/api-backend/vulkan/vulkan-only/vulkan_context.h>



namespace TheEngine::RenderingSystem::VulkanBackend
{
	void VulkanSwapchainManager::destroySwapchainImageViews(std::vector<VkImageView>& vkImageViews)
	{


		for (auto& imageView : vkImageViews)
		{
			//Check for failure
			vkDestroyImageView(m_vulkanContext.vkDevice, imageView, nullptr);
		}



		vkImageViews.resize(0); //Important

	}




	VkSwapchainKHR VulkanSwapchainManager::createVkSwapchainKHR(VkSwapchainKHR oldVkSwapchainKHR,const int width, const int height,const int imageCount)
	{


		
		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_vulkanContext.vkSurfaceKHR;

		createInfo.minImageCount = imageCount; 
		createInfo.imageFormat = VK_FORMAT_R8G8B8A8_SRGB; //TODO : Dont hardcode this
		createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; //TODO : Dont hardcode this

		createInfo.imageExtent = { (unsigned int)(width),(unsigned int)(height) };

		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR; // vsync //TODO : Dont hardcode this

		createInfo.clipped = VK_TRUE;

		createInfo.oldSwapchain = oldVkSwapchainKHR;

		VkSwapchainKHR newVkSwapchainKHR;

		if (vkCreateSwapchainKHR(m_vulkanContext.vkDevice, &createInfo, nullptr, &newVkSwapchainKHR) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}


		// Only destroy the old one AFTER the new one is successfully created
		if (oldVkSwapchainKHR != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(m_vulkanContext.vkDevice, oldVkSwapchainKHR, nullptr);
		}



		return newVkSwapchainKHR;

	}




	std::vector<VkImage> VulkanSwapchainManager::getSwapchainVkImage(VkSwapchainKHR vkSwapchainKHR)
	{
		uint32_t imageCount = 0;

		vkGetSwapchainImagesKHR(m_vulkanContext.vkDevice, vkSwapchainKHR, &imageCount, nullptr);

		std::vector<VkImage> swapchainVkImages(imageCount);

		auto result = vkGetSwapchainImagesKHR(m_vulkanContext.vkDevice, vkSwapchainKHR, &imageCount, swapchainVkImages.data());

		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to get swap chain images!");
		}


		return swapchainVkImages;
	}




	std::vector<VkImageView> VulkanSwapchainManager::createSwapchainImageViews(std::vector<VkImage>& swapchainVkImages)
	{
		const size_t imgCount = swapchainVkImages.size();

		
		std::vector<VkImageView> swapChainImageViews(imgCount);

		for (size_t i = 0; i < swapchainVkImages.size(); i++)
		{
			VkImageViewCreateInfo viewInfo{};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = swapchainVkImages[i];
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB; //TODO : Dont hardcode this
			viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;
			

			if (vkCreateImageView(m_vulkanContext.vkDevice, &viewInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create image views!");
			}
		}

		return swapChainImageViews;
	}


	VulkanSwapchainManager::VulkanSwapchainManager(VulkanContext& vulkanContext) :
		m_vulkanContext(vulkanContext)
	{



	}

	VulkanSwapchainManager::~VulkanSwapchainManager()
	{

		//TODO : cleanup VkSwapchainKHR and ImageViews , DO NOT DESTROY Swapchain VkImage
		vkDeviceWaitIdle(m_vulkanContext.vkDevice);
		destroySwapchainImageViews(m_swapchainVkImageViews);
		vkDestroySwapchainKHR(m_vulkanContext.vkDevice, m_vkSwapchainKHR, nullptr);

	}



	void VulkanSwapchainManager::recreateSwapchain(const uint32_t physicalWidth, const uint32_t physicalHeigth)
	{

		// Wait till gpu idle , I might need to keep in mind situations where it can lead to dead locks 
		vkDeviceWaitIdle(m_vulkanContext.vkDevice);

		// Note: Do not destroy swapchainImage we are not owners.
		// Note : Destroy swapchainImangeViews we are owners.
		destroySwapchainImageViews(m_swapchainVkImageViews);

		//passing the old one for optimization
		VkSwapchainKHR oldSwapchain = m_vkSwapchainKHR;
		m_vkSwapchainKHR = createVkSwapchainKHR(oldSwapchain, physicalWidth, physicalHeigth, 2);//this method will take care of old swapchain handle



		//create new ones
		m_swapchainVkImages = getSwapchainVkImage(m_vkSwapchainKHR);
		m_swapchainVkImageViews = createSwapchainImageViews(m_swapchainVkImages);
	}



	const VkSwapchainKHR& VulkanSwapchainManager::getVkSwapchainKHR() const
	{
		return m_vkSwapchainKHR;
	}


	void VulkanSwapchainManager::setCurrentImageAquiredIndex(const int index)
	{
		m_currentImageAquiredIndex = index;

	}



	VulkanImageViewCombined VulkanSwapchainManager::getVulkanImageViewCombinedSwapchain() const
	{
		return VulkanImageViewCombined{ m_swapchainVkImages[m_currentImageAquiredIndex], m_swapchainVkImageViews[m_currentImageAquiredIndex] };
	}

	VulkanImageViewCombined VulkanSwapchainManager::getVulkanImageViewCombinedSwapchain(const int index) const
	{
		//assert()
		return VulkanImageViewCombined{ m_swapchainVkImages[index], m_swapchainVkImageViews[index] };
	}






}