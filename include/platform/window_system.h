#pragma once

#include <cstdint>
#include <memory>
#include <string>



namespace TheEngine::RenderingSystem
{
	class IRenderDevice;
}

namespace TheEngine
{
	struct EngineConfiguration;
}

class SDL_Window;


namespace TheEngine::Platform
{



	class WindowSystem
	{

	private:


        std::string m_gpuVendor = "Unknown GPU Vendor";

		//use a smart pointer
        SDL_Window* m_window = nullptr;
	   
		//NOT OWNER
		std::unique_ptr<TheEngine::RenderingSystem::IRenderDevice> m_renderDevice;

   


	public:


        //creates window and GPU API context
		WindowSystem(const EngineConfiguration& engineConfiguration);
		~WindowSystem();

	


		std::unique_ptr<TheEngine::RenderingSystem::IRenderDevice> getRenderDevice();






	};





}