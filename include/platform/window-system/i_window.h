#pragma once
#include <engine/engine_core_data_types.h>
#include <memory>
#include <rendering-system/rhi/i_render_device.h>

namespace TheEngine::Platform
{
	class IWindow
	{

	private:


	public:

		virtual WindowExtent getWindowExtent() const = 0;
		virtual RenderingSystem::IRenderDevice& getRenderDevice() = 0;




	};



}