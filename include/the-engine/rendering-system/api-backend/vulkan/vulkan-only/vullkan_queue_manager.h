#pragma once
#include <volk.h>


namespace TheEngine::RenderingSystem::VulkanBackend
{
	class VulkanContext;
	enum class QueueType : uint8_t
	{
		UNKNOWN = 0,
		GRAPHICS = 1 << 0,
		COMPUTE = 1 << 1,
		TRANSFER = 1 << 2,
		PRESENT = 1 << 3,


		UNIFIED = GRAPHICS | COMPUTE | TRANSFER | PRESENT
	};

	inline QueueType operator|(QueueType lhs, QueueType rhs)
	{
		return static_cast<QueueType>(
			static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs)
			);
	}


	inline bool operator&(QueueType lhs, QueueType rhs)
	{
		return static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs);
	}

	struct QueueAddress
	{
		uint32_t queueFamilyIndex = 0;
		VkQueue vkQueue;
	};

	class VulkanQueueManager
	{
	private:
		
		VulkanContext& m_vulkanContext;

		static VkQueueFlags toVkQueueFlags(const TheEngine::RenderingSystem::VulkanBackend::QueueType type);
		
		//uint32_t getQueueFamilyIndex(const TheEngine::RenderingSystem::VulkanBackend::QueueType queueType);
		//VkQueue getVkQueue(const TheEngine::RenderingSystem::VulkanBackend::QueueType queueType);
	public:


		VulkanQueueManager(VulkanContext& vulkanContext);
		~VulkanQueueManager();

		QueueAddress getQueueFamilyAddress(const TheEngine::RenderingSystem::VulkanBackend::QueueType queueType) const;



	};





}
