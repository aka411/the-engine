#pragma once
#include "i_command_buffer.h"

namespace TheEngine::RenderingSystem
{
	//DO I need this in agnostic layer

	class ICommandBufferManager
	{

	private:


	

	public:

		ICommandBufferManager();

		//Need more thought
		virtual ICommandBuffer& getCommandBuffer() = 0;

	};












}