#pragma once

#include <cstdint>
#include "SDL3/SDL_video.h"
#include <memory>
#include <engine/engine_core_data_types.h>

namespace TheEngine::Platform
{




	class WindowSystem
	{

	private:


        std::string m_gpuVendor = "Unknown GPU Vendor";

		//use a smart pointer
        SDL_Window* m_window = nullptr;

        void enableOpenGLDebugging();


	public:


        //creates window and GPU API context
		WindowSystem(const EngineConfiguration& engineConfiguration);


		std::string getGPUVendor() const { return m_gpuVendor; };


		//swaps the front and back buffers, in opengl SDL does it for us , in vulkan its our job to do it
		void swapBuffers();



	};





}