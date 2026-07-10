#pragma once
#include <volk.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{

	void insertBarrierFromShaderWriteToShaderRead(VkCommandBuffer& vkCommandBuffer, VkImage vkImage);
	void insertBarrierFromShaderReadToShaderWrite(VkCommandBuffer& vkCommandBuffer, VkImage vkImage);
	
	void prepareImageForDrawing(VkCommandBuffer cmd, VkImage image); 
	void prepareImageForPresent(VkCommandBuffer cmd, VkImage image);

}