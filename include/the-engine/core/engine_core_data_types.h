#pragma once
#include <cstdint>
#include <string>
#include <array>


#ifdef PLATFORM_ANDROID
struct android_app;
#endif

namespace TheEngine
{

	enum class OS
	{
		SYS_WINDOWS,
		SYS_LINUX,
		SYS_ANDROID,
		SYS_UNKNOWN
	};

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

	
		std::string windowName{};



#ifdef PLATFORM_ANDROID
		android_app* androidApp{ nullptr };
#endif
	};




}