#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <engine/engine_core_data_types.h>



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
	   
		//NOT OWNER (may need design change)
		std::unique_ptr<TheEngine::RenderingSystem::IRenderDevice> m_renderDevice;

   


	public:



		WindowSystem(const EngineConfiguration& engineConfiguration);
		~WindowSystem();

	
		WindowExtent getWindowExtent() const;

		std::unique_ptr<TheEngine::RenderingSystem::IRenderDevice> getRenderDevice();






	};





}