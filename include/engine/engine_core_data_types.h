#pragma once
#include <string>
#include <array>

namespace TheEngine
{
	enum class RenderingAPI
	{

		OPENGL_4_6,
		VULKAN_1_3
	};


	struct WindowExtent
	{
		// The size in screen coordinates (logical), similar to css pixels
		uint32_t width = 0;
		uint32_t height = 0;

		// The size in actual pixels (physical) , the framebuffer size, the size considering Device Pixel Density(or is it dpi)
		//Frame buffer Size
		uint32_t physicalWidth = 0;
		uint32_t physicalHeight = 0;
	};

	struct EngineConfiguration
	{

		TheEngine::RenderingAPI renderingAPI;

		//This is the ;ogical pixels not frame buffer size
		int windowWidth;
		int windowHeight;

	
		std::string windowName;



		std::array<std::string, 3> mountPaths;

		//Window mode maybe
		//maybe even number of swapchains
	};




}