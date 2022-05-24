#include "Window.h"
#include "Input.h"
#include "GL.h"
#include <SDL2/SDL_image.h>
#include <iostream>

namespace sb
{
	Window::Window(int width, int height) 
		: m_isOpen(true), m_minimumMsPerFrame(0), m_camera(*this)
	{
		SB_ERROR_IF(SDL_Init(SDL_INIT_VIDEO) < 0, SDL_GetError());

		int flags = IMG_INIT_PNG & IMG_INIT_JPG;
		int imgResult = IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & flags;
		SB_ERROR_IF(imgResult != flags, IMG_GetError());

		#ifdef WIN32
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) == 0);
			m_sdlWindow = SDL_CreateWindow("Sandbox", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
			SDL_CHECK(m_sdlWindow); 
			m_glContext = SDL_GL_CreateContext(m_sdlWindow);
			SDL_CHECK(m_glContext);
			GLenum glewError = glewInit();

		#elif defined(__ANDROID__)
			SDL_DisplayMode mode;
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) == 0);
			SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24) == 0);
			SDL_CHECK(SDL_GetDisplayMode(0, 0, &mode) == 0);
			width = mode.w; height = mode.h;
			m_sdlWindow = SDL_CreateWindow("Sandbox", 0, 0, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_SHOWN);
			SDL_CHECK(m_sdlWindow);
			m_glContext = SDL_GL_CreateContext(m_sdlWindow);
			SDL_CHECK(m_glContext);
		#endif

		m_size = sb::Vector2i(width, height);
        m_camera.requestSize((float)width, (float)height);

		// the renderer expects an opened opengl context, so make sure it is created in the proper place, i.e.: here!
		m_renderer = new Renderer();		
		
		GL_CHECK(glDisable(GL_DEPTH_TEST));
		setSize(width, height);
	}

	Window::~Window()
	{
		SDL_DestroyWindow(m_sdlWindow);
		SDL_Quit();
	}

    Uint32 Window::getWindowId() const
    {
        return SDL_GetWindowID(m_sdlWindow);
    }

    void Window::setSize(int width, int height)
    {
        SDL_SetWindowSize(m_sdlWindow, width, height);
        onResize();
    }

    void Window::setFramerateLimit(int framerateLimit)
    {
        if (framerateLimit == 0)
            m_minimumMsPerFrame = 0;
        else
            m_minimumMsPerFrame = 1000.0f / framerateLimit;
    }

    void Window::update()
	{
		if (Input::hasQuitEvent())
			m_isOpen = false;

        if (Input::isWindowResizing(*this))
            onResize();
	}

	void Window::clear(const Color& clearColor)
	{
		GL_CHECK(glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a));
		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
	}

	void Window::draw(const std::vector<Vertex>& vertices, const PrimitiveType& primitiveType, const DrawStates& states)
	{
		Transform cameraTransform;

		DrawStates fullStates = states;
		fullStates.transform = m_camera.getTransform() * fullStates.transform;

		m_renderer->render(vertices, primitiveType, fullStates);
	}

	void Window::display()
	{
		SDL_GL_SwapWindow(m_sdlWindow);
        limitFramerate();
	}

    void Window::onResize()
    {
        SDL_GetWindowSize(m_sdlWindow, &m_size.x, &m_size.y);
        glViewport(0, 0, m_size.x, m_size.y);
        m_camera.refreshSize();
    }

    void Window::limitFramerate()
    {
        if (m_minimumMsPerFrame != 0) {
            float delay = m_minimumMsPerFrame - m_stopwatch.getElapsedMs();
            if (delay > 0)
                SDL_Delay((int)delay);
            m_stopwatch.reset();
        }
    }
}