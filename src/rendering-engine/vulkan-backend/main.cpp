#pragma once

#include <SDL3/SDL.h>
#include <iostream>
#include <SDL3/SDL_vulkan.h>

#include <volk.h>
#include <VkBootstrap.h>
#include <utlis.h>
#include <vulkan_renderer.h>




SDL_Window* createSDLWindow(int width, int height)
{

	// Initialize SDL3. If initialization fails, print an error and exit.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		logger("SDL could not initialize! SDL_Error: ");
		std::cout << SDL_GetError() << std::endl;
		return nullptr;
	}

	// Create an SDL3 window.
	// The window will be titled "SDL3 Window", positioned centrally,
	// with dimensions 800x600 pixels, and resizable.
	SDL_Window* window = SDL_CreateWindow(
		"Vukan Renderer",
		width, height,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN
	);

	if (window == nullptr)
	{
		logger("Window could not be created! SDL_Error: ");
		std::cout << SDL_GetError() << std::endl;
		SDL_Quit();

	}

	return window;

}















int main(int argc, char* argv[])
{

	const size_t WIDTH = 400;
	const size_t HEIGHT = 400;

	SDL_Window* window = createSDLWindow(WIDTH, HEIGHT);





	// Main loop flag
	bool quit = false;
	// Event handler
	SDL_Event e;

	volkInitialize();




	uint32_t extensionCount = 0;
	const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);

	vkb::InstanceBuilder builder;

	builder.set_app_name("Vulkan Renderer")
		.request_validation_layers(true)
		.require_api_version(1, 3, 0)
		.use_default_debug_messenger()
		.enable_extensions(extensionCount, extensions);

	auto instRet = builder.build();

	if (!instRet)
	{
		std::cout << "Failed to create Vulkan instance: " << instRet.error().message() << std::endl;
		return -1;
	}

	vkb::Instance instance = instRet.value();






	volkLoadInstanceOnly(instance.instance);

	VkSurfaceKHR surface;

	if (!SDL_Vulkan_CreateSurface(window, instance.instance, nullptr, &surface))
	{
		logger("Failed to create Vulkan surface.");
		std::cout << "Failed to create Vulkan instance: " << instRet.error().message() << std::endl;
		return -1;
	}




	VulkanRenderer vulkanRenderer(instance, surface);

	vulkanRenderer.setViewPort(WIDTH, HEIGHT);


	while (!quit)
	{
		// Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			// User requests quit
			if (e.type == SDL_EVENT_QUIT)
			{
				quit = true;
			}


			if (e.type == SDL_EVENT_WINDOW_RESIZED) 
				
			{
				
				
					// Get the new width and height from the event data
					int newWidth = e.window.data1;
					int newHeight = e.window.data2;

					// Call your function to handle the resize (e.g., recreate swap chain)
					vulkanRenderer.setViewPort(newWidth, newHeight);
				
			}


		}



		// run rendering code here
		vulkanRenderer.renderFrame();

	}

	// Destroy window
	SDL_DestroyWindow(window);

	// Quit SDL subsystems
	SDL_Quit();

	return 0;
}
