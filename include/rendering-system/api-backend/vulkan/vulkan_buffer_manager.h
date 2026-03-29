#pragma once

namespace TheEngine::RenderingSystem::VulkanBackend
{


	class VulkanBufferManager : public IBufferManager
	{

	private :


	public :
	
		VulkanBufferManager();
		~VulkanBufferManager();

		virtual const BufferHandle createBuffer(const GPUBufferCreateInfo& gpuBufferCreateInfo) override;



		virtual void destroyBuffer(const BufferHandle& bufferHandle) override;

	};






}