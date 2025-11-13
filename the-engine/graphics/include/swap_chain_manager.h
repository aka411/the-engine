#pragma once
#include "volk.h"
#include <vulkan_device.h>

struct ImageViewCombined
{
	VkImage image = VK_NULL_HANDLE;
	VkImageView imageView = VK_NULL_HANDLE;
	//VkFormat imageFormat = VK_FORMAT_UNDEFINED;
};


struct ViewPortDimension
{
	size_t width = 0;
	size_t height = 0;
};

class SwapChainManager
{
private:

	enum class SwapChainState
	{
		// Initial state: No resources created.
		PENDING_INITIAL_CREATION,

		// The swap chain is valid and ready to be used.
		READY,

		// A resize or setting change requires a new swap chain to be built.
		NEEDS_RECREATION
	};

	SwapChainState m_swapChainState = SwapChainState::PENDING_INITIAL_CREATION;

	VulkanDevice& m_vulkanDevice;

	VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

	ViewPortDimension m_viewPortDimension;


	std::vector<VkImage> m_swapChainImages;
	std::vector<VkImageView> m_swapChainImageViews;



	void createImage();
	void createimageViews();

	void createSwapChain(VkSwapchainKHR swapchain); // call this once at the start

	void recreateSwapChainAndResources();

	void destroySwapChain(VkSwapchainKHR swapchain);
	void destroyImageViews();
public:

	SwapChainManager(VulkanDevice& vulkanDevice);
	~SwapChainManager();


	void setViewportDimensions(uint32_t width, uint32_t height);
	void ensureReadyForDrawing(); // call before every frame

	ViewPortDimension getViewportDimensions();

	 const VkSwapchainKHR&  getSwapChain() const ;

	VkImageView getSwapChainImageView(int index);//need more thought
	VkImage getSwapChainImage(int index);
};
