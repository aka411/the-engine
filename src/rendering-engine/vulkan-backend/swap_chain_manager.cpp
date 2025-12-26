#include "swap_chain_manager.h"


void SwapChainManager::createImage()
{

	uint32_t imgCount = 0;
	vkGetSwapchainImagesKHR(m_vulkanDevice.getLogicalDeviceHandle(), m_swapChain, &imgCount, nullptr);

	m_swapChainImages.resize(imgCount);

	auto result = vkGetSwapchainImagesKHR(m_vulkanDevice.getLogicalDeviceHandle(), m_swapChain, &imgCount, m_swapChainImages.data());

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to get swap chain images!");
	}

}

void SwapChainManager::createimageViews()
{
	const size_t imgCount = m_swapChainImages.size();
	//create image views for each image
	m_swapChainImageViews.resize(imgCount);

	for (size_t i = 0; i < imgCount; i++)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = m_swapChainImages[i];
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = VK_FORMAT_B8G8R8A8_SRGB; //TODO : Dont hardcode this
		viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;


		if (vkCreateImageView(m_vulkanDevice.getLogicalDeviceHandle(), &viewInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create image views!");
		}
	}


}

SwapChainManager::SwapChainManager(VulkanDevice& vulkanDevice) : m_vulkanDevice(vulkanDevice)
{


}

SwapChainManager::~SwapChainManager()
{ 
	//owner of swapchain and image views and images
	//cleanup image views
	//TODO : cleanup swapchain

}

void SwapChainManager::setViewportDimensions(uint32_t width, uint32_t height)
{
	assert(width != 0);
	assert(height != 0);


	m_viewPortDimension.width = width;
	m_viewPortDimension.height = height;


	switch (m_swapChainState)
	{
		//Go from ready state to needs recreation state
	case SwapChainState::READY :
		m_swapChainState = SwapChainState::NEEDS_RECREATION;
		break;
	default:
		break;
	}

}

void SwapChainManager::ensureReadyForDrawing()
{
	switch (m_swapChainState)
	{
	case SwapChainState::PENDING_INITIAL_CREATION:
		createSwapChain(VK_NULL_HANDLE);
		createImage();
		createimageViews();
		m_swapChainState = SwapChainState::READY;
		break;

	case SwapChainState::NEEDS_RECREATION:
		recreateSwapChainAndResources();
		m_swapChainState = SwapChainState::READY;
		break;

	default:

		break;
	}




}

void SwapChainManager::createSwapChain(VkSwapchainKHR oldSwapchain)
{
	


	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = m_vulkanDevice.getSurface();
	createInfo.minImageCount = 2; // double buffering,//TODO : Dont hardcode this
	createInfo.imageFormat = VK_FORMAT_B8G8R8A8_SRGB; //TODO : Dont hardcode this
	createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR; //TODO : Dont hardcode this
	createInfo.imageExtent = { (unsigned int)(m_viewPortDimension.width),(unsigned int)( m_viewPortDimension.height) };
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR; // vsync //TODO : Dont hardcode this
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = oldSwapchain;//why is this needed? //TODO : look into this

	if (vkCreateSwapchainKHR(m_vulkanDevice.getLogicalDeviceHandle(), &createInfo, nullptr, &m_swapChain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	

}

void SwapChainManager::recreateSwapChainAndResources()
{
	//TODO : implement this

	// wait for all image views to return 
	 vkDeviceWaitIdle(m_vulkanDevice.getLogicalDeviceHandle());

	
	 VkSwapchainKHR oldSwapChain = m_swapChain;

	
	 createSwapChain(m_swapChain);


	 //destroy old swapchain and imageviews
	 destroySwapChain(oldSwapChain);
	 destroyImageViews();

	 createImage();
	 createimageViews();
	
	
}

void SwapChainManager::destroySwapChain(VkSwapchainKHR swapchain)
{

	vkDestroySwapchainKHR(m_vulkanDevice.getLogicalDeviceHandle(), swapchain, nullptr);
}

void SwapChainManager::destroyImageViews()
{

	for (auto& imageView : m_swapChainImageViews)
	{
		vkDestroyImageView(m_vulkanDevice.getLogicalDeviceHandle(), imageView, nullptr);
	}

	m_swapChainImageViews.resize(0); //Important
}






ViewPortDimension SwapChainManager::getViewportDimensions()
{
	return m_viewPortDimension;
}

 const VkSwapchainKHR& SwapChainManager::getSwapChain () const
{
	return m_swapChain;
}

VkImageView SwapChainManager::getSwapChainImageView(int index)
{
	return m_swapChainImageViews[index];
}

VkImage SwapChainManager::getSwapChainImage(int index)
{
	return m_swapChainImages[index];
}