#pragma once
#include <VkBootstrap.h>
// ORDER OF INCLUDING VOLK AND VMA HEADER IMPORTANT
#include <volk.h> 

#include "vk_mem_alloc.h"



class VulkanDevice
{
private:
	
	VolkDeviceTable m_volkDeviceTable;

	vkb::Instance m_instance;
	VkSurfaceKHR m_surface;

	vkb::PhysicalDevice m_physicalDevice;
	vkb::Device m_logicalDevice;

	VmaAllocator m_vmaAllocator;





public : 

	VulkanDevice(vkb::Instance  instance ,VkSurfaceKHR surface);

	//need to consider const correctness
	vkb::Device getVKBLogicalDevice() { return m_logicalDevice; }
	vkb::PhysicalDevice getVKBPhysicalDevice() { return m_physicalDevice; }

	const VkDevice& getLogicalDeviceHandle() const { return m_logicalDevice.device; }
	const VkPhysicalDevice& getPhysicalDeviceHandle() const { return m_physicalDevice.physical_device; }
	const VkInstance& getInstance() const { return m_instance.instance; }
	const VkSurfaceKHR& getSurface() const { return m_surface; }

     VmaAllocator& getVmaAllocator()  { return m_vmaAllocator; }
	~VulkanDevice();



};