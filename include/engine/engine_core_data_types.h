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

	struct EngineConfiguration
	{
		//API to Use
		TheEngine::RenderingAPI renderingAPI;

		//Window window sizes
		int windowWidth;
		int windowHeight;

		//window name 
		std::string windowName;



		std::array<std::string, 3> mountPaths;

		//Window mode maybe
		//maybe even number of swapchains
	};




}