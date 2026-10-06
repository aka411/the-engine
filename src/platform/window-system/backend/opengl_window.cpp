#include <platform/window-system/backend/opengl_window.h>
#include <assert.h>

#include <platform/window-system/i_window.h>


namespace TheEngine::Platform
{


	OpenGLWindow::OpenGLWindow(std::string_view name, const size_t width, const size_t height)
	{
		/*
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

m_window = SDL_CreateWindow(
	"TheEngine(openGL 4.6)",
	engineConfiguration.logicalWindowExtend.width, engineConfiguration.logicalWindowExtend.height,
	SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
);



SDL_GLContext  gl_context = SDL_GL_CreateContext(m_window);

SDL_GL_MakeCurrent(m_window, gl_context);


gladLoadGL();
gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);




const GLubyte* rawString = glGetString(GL_RENDERER);

if (rawString != nullptr)
{
	m_gpuVendor = reinterpret_cast<const char*>(rawString);
}

m_renderDevice = std::make_unique<TheEngine::RenderingSystem::OpenGLBackend::OpenglRenderDevice>();

*/

	}

	OpenGLWindow::~OpenGLWindow()
	{

	}

	/*
	 WindowExtent OpenGLWindow::getWindowExtent() const
	{
		 assert(false);
		 return WindowExtent{};
	}

	RenderingSystem::IRenderDevice& OpenGLWindow::getRenderDevice()
	{
		assert(false);
		return ;
	}


	*/



}