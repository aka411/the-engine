#pragma once
#include <cstdint>
#include <string>
#include <array>

namespace TheEngine
{
	enum class RenderingAPI
	{

		OPENGL_4_6,
		VULKAN_1_3
	};



	struct Extend2D
	{
		uint32_t width{ 0 };
		uint32_t height{ 0 };
	};

	struct WindowExtent
	{
		Extend2D logical{};
		Extend2D framebuffer{};
	};



	struct EngineConfiguration
	{

		TheEngine::RenderingAPI renderingAPI{ RenderingAPI::VULKAN_1_3};

		
		Extend2D logicalWindowExtend{};

	
		std::string windowName;



		std::array<std::string, 3> mountPaths;

		//Window mode maybe
		//maybe even number of swapchains
	};




}