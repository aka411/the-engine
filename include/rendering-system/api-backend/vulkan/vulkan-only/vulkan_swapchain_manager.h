#pragma once
#include <volk.h>
#include <vector>
#include <rendering-system/api-backend/vulkan/vulkan_texture_system_data_types.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{
	//OWNER : VulkanPresentationSystem


	class VulkanContext;

	class VulkanSwapchainManager
	{


	private:

		VulkanContext& m_vulkanContext;

		VkSwapchainKHR m_vkSwapchainKHR = VK_NULL_HANDLE;

		std::vector<VkImage> m_swapchainVkImages;// NOT OWNER, OS IS OWNER!!!
		std::vector<VkImageView> m_swapchainVkImageViews;//OWNER, WE HAVE TO MANAGE ITS LIFE


		int m_imageCount = 0;

		int m_currentImageAquiredIndex = 0;


		//cleanup
		void destroySwapchainImageViews(std::vector<VkImageView>& vkImageViews);



		VkSwapchainKHR createVkSwapchainKHR(VkSwapchainKHR oldVkSwapchainKHR, const int width, const int height, const int imageCount);//old VkSwapchainKHR will be destroyed inside this

		std::vector<VkImage> getSwapchainVkImage(VkSwapchainKHR vkSwapchainKHR);
		std::vector<VkImageView> createSwapchainImageViews(std::vector<VkImage>& swapchainVkImages);

		
	public:

		VulkanSwapchainManager(VulkanContext& vulkanContext);

		~VulkanSwapchainManager();

		void recreateSwapchain(const uint32_t physicalWidth, const uint32_t physicalHeigth);


		

		//Note : can become stale, dont store
		VulkanImageViewCombined getVulkanImageViewCombinedSwapchain() const;//using the current set
		VulkanImageViewCombined getVulkanImageViewCombinedSwapchain(const int index) const;

		//Note : can become stale, dont store
		const VkSwapchainKHR& getVkSwapchainKHR() const;

		void setCurrentImageAquiredIndex(const int index);
	};






}