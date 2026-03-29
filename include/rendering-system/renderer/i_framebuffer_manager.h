#pragma once


namespace TheEngine::RenderingSystem
{

	//NO NEED FOR INTERFACE. IN VULKAN IT WILL CREATE THE RENDERINFO, IN OPENGL WILL SET STATE thats all
	class IFramebufferManager
	{


	private:



	public:

		IFramebufferManager();
		virtual ~IFramebufferManager() = default;



	};





}