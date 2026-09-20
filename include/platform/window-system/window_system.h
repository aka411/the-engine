#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <engine/engine_core_data_types.h>
#include <platform/window-system/i_window.h>


namespace TheEngine::RenderingSystem
{
	class IRenderDevice;
}

namespace TheEngine
{
	struct EngineConfiguration;
}

namespace TheEngine::Platform
{


	class IWindow;
	class WindowSystem
	{

	private:

		std::unique_ptr<IWindow> m_window;

	public:



		WindowSystem(const EngineConfiguration& engineConfiguration);
		~WindowSystem();

	
		WindowExtent getWindowExtent() const;

		IWindow& getWindow();

		RenderingSystem::IRenderDevice& getRenderDevice();

	};





}