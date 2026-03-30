#pragma once

namespace TheEngine::RenderingSystem
{

	class ICommandBufferManager
	{

	private:


	

	public:

		ICommandBufferManager();


		virtual ICommandBuffer& getICommandBuffer() = 0;

	};












}