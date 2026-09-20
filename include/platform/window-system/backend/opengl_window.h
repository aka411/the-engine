#pragma once
#include <platform/window-system/i_window.h>

namespace TheEngine::Platform
{

	class OpenGLWindow //: public IWindow
	{

	private:


	public:

		OpenGLWindow(std::string_view name, const size_t width, const size_t height);
		~OpenGLWindow();

		//virtual WindowExtent getWindowExtent() const override;
		//virtual RenderingSystem::IRenderDevice& getRenderDevice() override;

	};





}